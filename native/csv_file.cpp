#include "csv_file.h"
#include "utils.h"

#include <algorithm>

namespace
{
  constexpr std::string_view BOM = "\xEF\xBB\xBF";
  constexpr std::string_view EOL = "\r\n";

  // Pagina de codigo usada pelo Excel pt-BR ao salvar CSV sem UTF-8.
  constexpr unsigned ANSI_CODEPAGE = 1252;

  CsvStatus from_file(FileStatus st)
  {
    switch (st)
    {
      case FileStatus::Ok:       return CsvStatus::Ok;
      case FileStatus::NotFound: return CsvStatus::NotFound;
      case FileStatus::Locked:   return CsvStatus::Locked;
      case FileStatus::IoError:  break;
    }
    return CsvStatus::IoError;
  }

  // Remove o BOM ou, se o arquivo nao for UTF-8 valido, converte de
  // Windows-1252 (arquivo salvo pelo Excel) para UTF-8.
  CsvStatus decode(std::string& data)
  {
    if (data.compare(0, BOM.size(), BOM) == 0)
    {
      data.erase(0, BOM.size());
      return CsvStatus::Ok;
    }

    std::wstring probe;
    if (utf8_to_wide(data, probe)) return CsvStatus::Ok;   // ja e UTF-8

    std::string converted;
    if (!codepage_to_utf8(data, ANSI_CODEPAGE, converted)) return CsvStatus::BadFormat;
    data.swap(converted);
    return CsvStatus::Ok;
  }

  void append_field(std::string& out, const std::string& field)
  {
    const bool quote =
      field.find_first_of(std::string{CsvFile::SEP, '"', '\r', '\n'}) != std::string::npos;

    if (!quote)
    {
      out += field;
      return;
    }

    out += '"';
    for (char c : field)
    {
      if (c == '"') out += '"';
      out += c;
    }
    out += '"';
  }

  void append_row(std::string& out, const CsvRow& row)
  {
    for (size_t i = 0; i < row.size(); ++i)
    {
      if (i) out += CsvFile::SEP;
      append_field(out, row[i]);
    }
    out += EOL;
  }

  bool is_blank(const CsvRow& row)
  {
    return std::all_of(row.begin(), row.end(), [](const std::string& f) { return f.empty(); });
  }
}  // namespace


// ----------------------------------------------------------------------------
// CsvFile
// ----------------------------------------------------------------------------

CsvStatus CsvFile::open(const fs::path& path, OpenMode mode, unsigned timeout_ms)
{
  return from_file(m_file.open(path, mode, timeout_ms));
}

CsvStatus CsvFile::read(CsvTable& out)
{
  out = {};

  std::string data;
  CsvStatus st = from_file(m_file.read_all(data));
  if (st != CsvStatus::Ok) return st;

  st = decode(data);
  if (st != CsvStatus::Ok) return st;

  return parse(data, out);
}

CsvStatus CsvFile::write(const CsvTable& table)
{
  return from_file(m_file.write_all(serialize(table)));
}

CsvStatus CsvFile::remove()
{
  return from_file(m_file.remove());
}

CsvStatus CsvFile::parse(std::string_view data, CsvTable& out)
{
  out = {};

  std::vector<CsvRow> rows;
  CsvRow row;
  std::string field;
  bool in_quotes = false;      // dentro de um campo entre aspas
  bool field_quoted = false;   // o campo atual comecou com aspas
  bool row_started = false;    // a linha atual tem algum conteudo

  auto end_field = [&]
  {
    row.push_back(std::move(field));
    field.clear();
    field_quoted = false;
  };

  const size_t n = data.size();
  size_t i = 0;
  while (i < n)
  {
    const char c = data[i];

    if (in_quotes)
    {
      if (c == '"')
      {
        if (i + 1 < n && data[i + 1] == '"')   // "" -> "
        {
          field += '"';
          i += 2;
          continue;
        }
        in_quotes = false;
        ++i;
        continue;
      }
      field += c;   // inclui ';' e quebras de linha
      ++i;
      continue;
    }

    if (c == '"' && field.empty() && !field_quoted)
    {
      in_quotes = true;
      field_quoted = true;
      row_started = true;
      ++i;
      continue;
    }

    if (c == SEP)
    {
      end_field();
      row_started = true;
      ++i;
      continue;
    }

    if (c == '\r' || c == '\n')
    {
      if (c == '\r' && i + 1 < n && data[i + 1] == '\n') ++i;
      ++i;
      if (row_started)
      {
        end_field();
        rows.push_back(std::move(row));
        row.clear();
      }
      row_started = false;
      continue;
    }

    field += c;
    row_started = true;
    ++i;
  }

  if (in_quotes) return CsvStatus::BadFormat;   // aspas nao fechadas
  if (row_started)
  {
    end_field();
    rows.push_back(std::move(row));
  }

  if (rows.empty()) return CsvStatus::Ok;   // arquivo vazio

  // O Excel pode deixar ';' sobrando no fim das linhas.
  out.header = std::move(rows.front());
  while (!out.header.empty() && out.header.back().empty()) out.header.pop_back();

  const size_t cols = out.header.size();
  for (size_t r = 1; r < rows.size(); ++r)
  {
    CsvRow& cur = rows[r];
    while (cur.size() > cols && cur.back().empty()) cur.pop_back();
    if (cur.size() > cols) return CsvStatus::BadFormat;   // dado fora de coluna
    cur.resize(cols);
    if (is_blank(cur)) continue;   // linha vazia (ex.: ";;;" do Excel)
    out.rows.push_back(std::move(cur));
  }

  return CsvStatus::Ok;
}

std::string CsvFile::serialize(const CsvTable& table)
{
  std::string out(BOM);
  append_row(out, table.header);
  for (const CsvRow& row : table.rows) append_row(out, row);
  return out;
}
