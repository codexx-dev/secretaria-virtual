#include "db.h"
#include "csv_file.h"
#include "utils.h"

#include <algorithm>
#include <string_view>

namespace
{
  constexpr unsigned LOCK_TIMEOUT_MS = 3000;

  DbStatus from_csv(CsvStatus st)
  {
    switch (st)
    {
      case CsvStatus::Ok:        return DbStatus::Ok;
      case CsvStatus::NotFound:  return DbStatus::NotFound;
      case CsvStatus::Locked:    return DbStatus::Locked;
      case CsvStatus::BadFormat: return DbStatus::BadFormat;
      case CsvStatus::IoError:   break;
    }
    return DbStatus::IoError;
  }

  // Nomes de colunas: ao menos um, nao vazios e sem repeticao.
  bool are_valid_names(const std::vector<std::string>& names)
  {
    if (names.empty()) return false;
    for (size_t i = 0; i < names.size(); ++i)
    {
      if (names[i].empty()) return false;
      for (size_t j = i + 1; j < names.size(); ++j)
        if (names[i] == names[j]) return false;
    }
    return true;
  }

  std::vector<std::string> names_of(const Row& row)
  {
    std::vector<std::string> names;
    for (const Field& f : row) names.push_back(f.first);
    return names;
  }

  // Poe a linha na ordem do cabecalho. Exige todas as colunas.
  DbStatus to_csv_row(const CsvRow& header, const Row& row, CsvRow& out)
  {
    if (!are_valid_names(names_of(row)) || row.size() != header.size())
      return DbStatus::InvalidColumns;

    out.assign(header.size(), "");
    for (const Field& f : row)
    {
      const auto it = std::find(header.begin(), header.end(), f.first);
      if (it == header.end()) return DbStatus::UnknownColumn;
      out[it - header.begin()] = f.second;
    }
    return DbStatus::Ok;
  }

  std::string_view trim(std::string_view s)
  {
    const size_t first = s.find_first_not_of(" \t");
    if (first == std::string_view::npos) return {};
    return s.substr(first, s.find_last_not_of(" \t") - first + 1);
  }

  bool same_row(const CsvRow& a, const CsvRow& b)
  {
    return std::equal(a.begin(), a.end(), b.begin(), b.end(),
      [](const std::string& x, const std::string& y) { return trim(x) == trim(y); });
  }

  std::vector<CsvRow>::iterator find_row(CsvTable& table, const CsvRow& row)
  {
    return std::find_if(table.rows.begin(), table.rows.end(),
      [&](const CsvRow& r) { return same_row(r, row); });
  }

  // Abre a tabela travada e le o conteudo.
  DbStatus open_table(const fs::path& path, OpenMode mode, CsvFile& file, CsvTable& table)
  {
    if (!is_inside_data_dir(path)) return DbStatus::InvalidName;

    DbStatus st = from_csv(file.open(path, mode, LOCK_TIMEOUT_MS));
    if (st != DbStatus::Ok) return st;

    return from_csv(file.read(table));
  }
}  // namespace


const char* db_status_message(DbStatus st)
{
  switch (st)
  {
    case DbStatus::Ok:             return "Operacao concluida.";
    case DbStatus::InvalidName:    return "Nome de tabela invalido.";
    case DbStatus::InvalidColumns: return "Colunas invalidas: vazias, repetidas ou ausentes.";
    case DbStatus::UnknownColumn:  return "A tabela nao possui uma das colunas informadas.";
    case DbStatus::NotFound:       return "Tabela nao encontrada.";
    case DbStatus::AlreadyExists:  return "A tabela ja existe.";
    case DbStatus::RowNotFound:    return "Linha nao encontrada. Ela pode ter sido alterada por outro programa.";
    case DbStatus::DuplicateRow:   return "Ja existe uma linha igual.";
    case DbStatus::Locked:         return "Arquivo em uso por outro programa (ex.: Excel) ou sem permissao.";
    case DbStatus::IoError:        return "Falha ao acessar o disco.";
    case DbStatus::BadFormat:      return "O arquivo .csv esta corrompido ou em formato invalido.";
  }
  return "Erro desconhecido.";
}

DbStatus DB::create(const fs::path& path, const std::vector<std::string>& columns)
{
  if (!is_inside_data_dir(path)) return DbStatus::InvalidName;
  if (!are_valid_names(columns)) return DbStatus::InvalidColumns;

  CsvFile file;
  DbStatus st = from_csv(file.open(path, OpenMode::CreateIfMissing, LOCK_TIMEOUT_MS));
  if (st != DbStatus::Ok) return st;
  if (!file.created()) return DbStatus::AlreadyExists;

  CsvTable table;
  table.header = columns;
  st = from_csv(file.write(table));
  if (st != DbStatus::Ok) file.remove();   // nao deixa arquivo vazio para tras
  return st;
}

DbStatus DB::remove(const fs::path& path)
{
  if (!is_inside_data_dir(path)) return DbStatus::InvalidName;

  CsvFile file;
  const DbStatus st = from_csv(file.open(path, OpenMode::Existing, LOCK_TIMEOUT_MS));
  if (st != DbStatus::Ok) return st;

  return from_csv(file.remove());
}

DbStatus DB::insert_val(const fs::path& path, const Row& row)
{
  // Validado antes de abrir: tabela nova nunca fica criada por coluna invalida.
  if (!are_valid_names(names_of(row))) return DbStatus::InvalidColumns;

  CsvFile file;
  CsvTable table;
  DbStatus st = open_table(path, OpenMode::CreateIfMissing, file, table);
  if (st != DbStatus::Ok) return st;

  if (table.header.empty()) table.header = names_of(row);   // tabela nova

  CsvRow csv_row;
  st = to_csv_row(table.header, row, csv_row);
  if (st != DbStatus::Ok) return st;
  if (find_row(table, csv_row) != table.rows.end()) return DbStatus::DuplicateRow;

  table.rows.push_back(std::move(csv_row));
  st = from_csv(file.write(table));
  if (st != DbStatus::Ok && file.created()) file.remove();   // nao deixa arquivo vazio
  return st;
}

DbStatus DB::update_val(const fs::path& path, const Row& old_row, const Row& new_row)
{
  CsvFile file;
  CsvTable table;
  DbStatus st = open_table(path, OpenMode::Existing, file, table);
  if (st != DbStatus::Ok) return st;

  CsvRow old_csv, new_csv;
  st = to_csv_row(table.header, old_row, old_csv);
  if (st != DbStatus::Ok) return st;
  st = to_csv_row(table.header, new_row, new_csv);
  if (st != DbStatus::Ok) return st;

  const auto it = find_row(table, old_csv);
  if (it == table.rows.end()) return DbStatus::RowNotFound;

  // Trocar a linha por ela mesma nao e duplicata.
  if (!same_row(old_csv, new_csv) && find_row(table, new_csv) != table.rows.end())
    return DbStatus::DuplicateRow;

  *it = std::move(new_csv);
  return from_csv(file.write(table));
}

DbStatus DB::delete_val(const fs::path& path, const Row& row)
{
  CsvFile file;
  CsvTable table;
  DbStatus st = open_table(path, OpenMode::Existing, file, table);
  if (st != DbStatus::Ok) return st;

  CsvRow csv_row;
  st = to_csv_row(table.header, row, csv_row);
  if (st != DbStatus::Ok) return st;

  const auto it = find_row(table, csv_row);
  if (it == table.rows.end()) return DbStatus::RowNotFound;

  table.rows.erase(it);
  return from_csv(file.write(table));
}
