#pragma once

#include "csv_file.h"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

namespace fs = std::filesystem;

// Par coluna/valor.
using Field = std::pair<std::string, std::string>;
using Row   = std::vector<Field>;

enum class DbStatus
{
  Ok,
  InvalidName,      // caminho da tabela fora da pasta de dados
  InvalidColumns,   // coluna vazia/repetida, ou linha sem todas as colunas
  UnknownColumn,    // coluna que nao existe no cabecalho do .csv
  NotFound,         // tabela nao existe
  AlreadyExists,    // create() em tabela que ja existe
  RowNotFound,      // linha nao existe (ou foi alterada por outro programa)
  DuplicateRow,     // ja existe uma linha igual
  Formula,          // texto que o Excel executaria como formula (=, +, -, @)
  Locked,           // arquivo em uso (ex.: aberto no Excel) ou sem permissao
  IoError,          // outra falha de disco
  BadFormat,        // .csv corrompido ou editado de forma invalida
};

// Mensagem em portugues para mostrar ao usuario.
const char* db_status_message(DbStatus st);

// "Banco de dados" de arquivos .csv. Cada tabela e um arquivo dentro de
// ServerConfig::data_dir(); caminho fora dela -> InvalidName.
//
// Nao ha chave: a linha inteira identifica a si mesma. Toda linha precisa
// trazer todas as colunas do cabecalho, e a comparacao ignora espacos nas
// pontas das celulas. A tabela nao aceita duas linhas iguais.
class DB
{
public:
  // Le a tabela inteira.
  static DbStatus read(const fs::path& path, CsvTable& out);

  // Cria a tabela so com o cabecalho.
  static DbStatus create(const fs::path& path, const std::vector<std::string>& columns);

  // Apaga o arquivo da tabela.
  static DbStatus remove(const fs::path& path);

  // Acrescenta a linha. Tabela inexistente e criada com as colunas da linha.
  static DbStatus insert_val(const fs::path& path, const Row& row);

  // Troca old_row por new_row.
  static DbStatus update_val(const fs::path& path, const Row& old_row, const Row& new_row);

  // Apaga a linha.
  static DbStatus delete_val(const fs::path& path, const Row& row);
};
