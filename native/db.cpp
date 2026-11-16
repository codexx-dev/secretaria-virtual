#include "db.h"
#include "csv_file.h"
#include "utils.h"

#include <algorithm>
#include <cstring>

namespace
{
  constexpr unsigned LOCK_TIMEOUT_MS = 3000;

  // O Windows 8 nao aceita caminhos longos (MAX_PATH = 260, contando o terminador).
  constexpr size_t MAX_PATH_LEN = 260;

  // Uma coluna do cabecalho e o valor a comparar/gravar nela.
  using ColumnValues = std::vector<std::pair<size_t, const std::string*>>;

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

  bool iequals_ascii(std::string_view a, std::string_view b)
  {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i)
    {
      char x = a[i], y = b[i];
      if (x >= 'a' && x <= 'z') x = static_cast<char>(x - 'a' + 'A');
      if (y >= 'a' && y <= 'z') y = static_cast<char>(y - 'a' + 'A');
      if (x != y) return false;
    }
    return true;
  }

  // CON, PRN, AUX, NUL, COM1-9, LPT1-9 (com ou sem extensao).
  bool is_reserved_name(std::string_view seg)
  {
    const std::string_view base = seg.substr(0, seg.find('.'));

    for (const char* r : {"CON", "PRN", "AUX", "NUL"})
      if (iequals_ascii(base, r)) return true;

    if (base.size() == 4 && base[3] >= '1' && base[3] <= '9')
    {
      const std::string_view prefix = base.substr(0, 3);
      if (iequals_ascii(prefix, "COM") || iequals_ascii(prefix, "LPT")) return true;
    }
    return false;
  }

  // Um trecho do nome da tabela (entre barras).
  bool is_valid_segment(std::string_view seg)
  {
    if (seg.empty() || seg == "." || seg == "..") return false;
    if (seg.size() > 255) return false;
    if (seg.front() == ' ') return false;
    if (seg.back() == ' ' || seg.back() == '.') return false;   // o Windows remove

    for (unsigned char c : seg)
    {
      if (c < 0x20) return false;
      if (std::strchr("<>:\"|?*", c)) return false;
    }

    return !is_reserved_name(seg);
  }

  // Nomes de colunas: nao vazios e sem repeticao.
  bool are_valid_names(const std::vector<std::string_view>& names)
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

  bool are_valid_names(const Record& record)
  {
    std::vector<std::string_view> names;
    names.reserve(record.size());
    for (const Field& f : record) names.push_back(f.first);
    return are_valid_names(names);
  }

  // Liga cada coluna do registro a sua posicao no cabecalho.
  DbStatus map_columns(const CsvRow& header, const Record& record, ColumnValues& out)
  {
    out.clear();
    for (const Field& f : record)
    {
      const auto it = std::find(header.begin(), header.end(), f.first);
      if (it == header.end()) return DbStatus::UnknownColumn;
      out.emplace_back(static_cast<size_t>(it - header.begin()), &f.second);
    }
    return DbStatus::Ok;
  }

  bool matches(const CsvRow& row, const ColumnValues& where)
  {
    for (const auto& [col, value] : where)
      if (row[col] != *value) return false;
    return true;
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
    case DbStatus::Locked:         return "Arquivo em uso por outro programa (ex.: Excel) ou sem permissao.";
    case DbStatus::IoError:        return "Falha ao acessar o disco.";
    case DbStatus::BadFormat:      return "O arquivo .csv esta corrompido ou em formato invalido.";
  }
  return "Erro desconhecido.";
}

DbStatus DB::resolve(std::string_view name, fs::path& out) const
{
  if (m_root.empty() || name.empty()) return DbStatus::InvalidName;

  fs::path path = m_root;
  size_t start = 0;
  for (;;)
  {
    const size_t pos = name.find_first_of("/\\", start);
    const std::string_view seg =
      name.substr(start, pos == std::string_view::npos ? std::string_view::npos : pos - start);

    if (!is_valid_segment(seg)) return DbStatus::InvalidName;

    std::wstring wide;
    if (!utf8_to_wide(seg, wide)) return DbStatus::InvalidName;
    path /= wide;

    if (pos == std::string_view::npos) break;
    start = pos + 1;
  }
  path += L".csv";

  if (path.native().size() >= MAX_PATH_LEN) return DbStatus::InvalidName;

  out = std::move(path);
  return DbStatus::Ok;
}

DbStatus DB::create(std::string_view name, const std::vector<std::string>& columns)
{
  fs::path path;
  DbStatus st = resolve(name, path);
  if (st != DbStatus::Ok) return st;

  if (!are_valid_names(std::vector<std::string_view>(columns.begin(), columns.end())))
    return DbStatus::InvalidColumns;

  CsvFile file;
  st = from_csv(file.open(path, OpenMode::CreateIfMissing, LOCK_TIMEOUT_MS));
  if (st != DbStatus::Ok) return st;
  if (!file.created()) return DbStatus::AlreadyExists;

  CsvTable table;
  table.header = columns;
  st = from_csv(file.write(table));
  if (st != DbStatus::Ok) file.remove();   // nao deixa arquivo vazio para tras
  return st;
}

DbStatus DB::remove(std::string_view name)
{
  fs::path path;
  DbStatus st = resolve(name, path);
  if (st != DbStatus::Ok) return st;

  CsvFile file;
  st = from_csv(file.open(path, OpenMode::Existing, LOCK_TIMEOUT_MS));
  if (st != DbStatus::Ok) return st;

  return from_csv(file.remove());
}

DbStatus DB::insert_val(std::string_view name, const Record& record)
{
  fs::path path;
  DbStatus st = resolve(name, path);
  if (st != DbStatus::Ok) return st;

  if (!are_valid_names(record)) return DbStatus::InvalidColumns;

  CsvFile file;
  st = from_csv(file.open(path, OpenMode::CreateIfMissing, LOCK_TIMEOUT_MS));
  if (st != DbStatus::Ok) return st;

  CsvTable table;
  st = from_csv(file.read(table));   // arquivo recem-criado -> tabela vazia
  if (st != DbStatus::Ok) return st;

  // Tabela nova (ou arquivo vazio): o primeiro registro define as colunas.
  if (table.header.empty())
    for (const Field& f : record) table.header.push_back(f.first);

  ColumnValues cols;
  st = map_columns(table.header, record, cols);
  if (st != DbStatus::Ok) return st;

  CsvRow row(table.header.size());
  for (const auto& [col, value] : cols) row[col] = *value;
  table.rows.push_back(std::move(row));

  st = from_csv(file.write(table));
  if (st != DbStatus::Ok && file.created()) file.remove();   // nao deixa arquivo vazio
  return st;
}

DbStatus DB::update_val(
  std::string_view name,
  const Record& where,
  const Record& values,
  size_t* affected)
{
  if (affected) *affected = 0;

  fs::path path;
  DbStatus st = resolve(name, path);
  if (st != DbStatus::Ok) return st;

  if (!are_valid_names(where) || !are_valid_names(values)) return DbStatus::InvalidColumns;

  CsvFile file;
  st = from_csv(file.open(path, OpenMode::Existing, LOCK_TIMEOUT_MS));
  if (st != DbStatus::Ok) return st;

  CsvTable table;
  st = from_csv(file.read(table));
  if (st != DbStatus::Ok) return st;

  ColumnValues filter, changes;
  if ((st = map_columns(table.header, where, filter)) != DbStatus::Ok) return st;
  if ((st = map_columns(table.header, values, changes)) != DbStatus::Ok) return st;

  size_t count = 0;
  for (CsvRow& row : table.rows)
  {
    if (!matches(row, filter)) continue;
    for (const auto& [col, value] : changes) row[col] = *value;
    ++count;
  }

  if (affected) *affected = count;
  if (count == 0) return DbStatus::Ok;   // nada a gravar

  return from_csv(file.write(table));
}

DbStatus DB::delete_val(std::string_view name, const Record& where, size_t* affected)
{
  if (affected) *affected = 0;

  fs::path path;
  DbStatus st = resolve(name, path);
  if (st != DbStatus::Ok) return st;

  if (!are_valid_names(where)) return DbStatus::InvalidColumns;

  CsvFile file;
  st = from_csv(file.open(path, OpenMode::Existing, LOCK_TIMEOUT_MS));
  if (st != DbStatus::Ok) return st;

  CsvTable table;
  st = from_csv(file.read(table));
  if (st != DbStatus::Ok) return st;

  ColumnValues filter;
  if ((st = map_columns(table.header, where, filter)) != DbStatus::Ok) return st;

  const auto first = std::remove_if(table.rows.begin(), table.rows.end(),
    [&](const CsvRow& row) { return matches(row, filter); });
  const size_t count = static_cast<size_t>(table.rows.end() - first);
  table.rows.erase(first, table.rows.end());

  if (affected) *affected = count;
  if (count == 0) return DbStatus::Ok;

  return from_csv(file.write(table));
}
