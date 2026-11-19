#pragma once

#include "locked_file.h"

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

// ----------------------------------------------------------------------------
// Formato dos arquivos .csv (compativel com o Excel em pt-BR):
//   - codificacao UTF-8 com BOM (EF BB BF);
//   - separador ';';
//   - fim de linha CRLF;
//   - campos com ';', aspas ou quebra de linha vao entre aspas, e aspas
//     internas sao duplicadas (RFC 4180).
//
// Na leitura tambem aceita o que o Excel grava ao "Salvar como CSV":
// arquivo sem BOM em Windows-1252, ';' sobrando no fim das linhas e linhas
// totalmente vazias.
// ----------------------------------------------------------------------------

using CsvRow = std::vector<std::string>;   // campos em UTF-8

struct CsvTable
{
  CsvRow header;               // nomes das colunas
  std::vector<CsvRow> rows;    // cada linha tem header.size() campos
};

enum class CsvStatus
{
  Ok,
  NotFound,    // arquivo ou pasta nao existe
  Locked,      // arquivo em uso (ex.: aberto no Excel) ou sem permissao
  IoError,     // outra falha de leitura/escrita
  BadFormat,   // conteudo nao e um CSV valido
};

// Arquivo .csv aberto e travado (LockedFile, de locked_file.h). Enquanto o
// CsvFile estiver aberto, nenhum outro programa consegue gravar no .csv,
// entao um ler-modificar-gravar fica protegido do inicio ao fim.
class CsvFile
{
private:
  LockedFile m_file;

public:
  static constexpr char SEP = ';';

  // Abre e trava o .csv (ver LockedFile::open).
  CsvStatus open(const fs::path& path, OpenMode mode, unsigned timeout_ms = 3000);
  void close() { m_file.close(); }

  // true se open() acabou de criar o arquivo (vazio).
  bool created() const { return m_file.created(); }

  // Arquivo vazio -> tabela vazia (sem cabecalho).
  CsvStatus read(CsvTable& out);

  // Substitui todo o conteudo do arquivo pela tabela.
  CsvStatus write(const CsvTable& table);

  // Apaga o arquivo e fecha.
  CsvStatus remove();

  // Conversao texto <-> tabela (sem acesso a disco).
  static CsvStatus parse(std::string_view utf8, CsvTable& out);
  static std::string serialize(const CsvTable& table);
};

// Tabela -> JSON: [{"coluna": "valor", ...}, ...], uma entrada por linha.
std::string csv_to_json(const CsvTable& table);
