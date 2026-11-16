#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace fs = std::filesystem;

// Par coluna/valor de um registro. Os dados chegam em tempo de execucao
// (do JSON da requisicao), por isso vector e nao initializer_list.
using Field  = std::pair<std::string, std::string>;
using Record = std::vector<Field>;

enum class DbStatus
{
  Ok,
  InvalidName,      // nome de tabela invalido ou perigoso (ex.: "../x")
  InvalidColumns,   // coluna vazia/repetida, registro ou filtro vazio
  UnknownColumn,    // coluna que nao existe no cabecalho do .csv
  NotFound,         // tabela nao existe
  AlreadyExists,    // create() em tabela que ja existe
  Locked,           // arquivo em uso (ex.: aberto no Excel) ou sem permissao
  IoError,          // outra falha de disco
  BadFormat,        // .csv corrompido ou editado de forma invalida
};

// Mensagem em portugues para mostrar ao usuario.
const char* db_status_message(DbStatus st);

// "Banco de dados" de arquivos .csv. Cada tabela e um arquivo:
//   "TRANSPORTE ESCOLAR/CRACHAS/LISTA DE ALUNOS"
//     -> <raiz>/TRANSPORTE ESCOLAR/CRACHAS/LISTA DE ALUNOS.csv
//
// Nomes de tabela e de colunas chegam em UTF-8. A DB nao conhece a
// estrutura das tabelas: as colunas sao as do cabecalho do .csv.
// Toda operacao mantem o .csv aberto e travado (CsvFile/LockedFile) do
// inicio ao fim, entao nenhum outro programa grava nele no meio do caminho.
class DB
{
private:
  fs::path m_root;

public:
  explicit DB(fs::path root) : m_root(std::move(root)) { }

  const fs::path& root() const { return m_root; }

  // Converte o nome da tabela no caminho do .csv, recusando nomes que
  // saiam da raiz, nomes reservados do Windows e caracteres invalidos.
  DbStatus resolve(std::string_view name, fs::path& out) const;

  // Cria a tabela so com o cabecalho.
  DbStatus create(std::string_view name, const std::vector<std::string>& columns);

  // Apaga o arquivo da tabela.
  DbStatus remove(std::string_view name);

  // Acrescenta uma linha. Se a tabela nao existir, ela e criada e as
  // colunas do registro viram o cabecalho. Colunas ausentes ficam vazias;
  // coluna que nao existe no cabecalho -> UnknownColumn.
  DbStatus insert_val(std::string_view name, const Record& record);

  // Altera as linhas em que todas as colunas de `where` tem exatamente os
  // valores indicados. `where` vazio e recusado (evita alterar tudo).
  DbStatus update_val(
    std::string_view name,
    const Record& where,
    const Record& values,
    size_t* affected = nullptr
  );

  // Apaga as linhas que casam com `where` (mesma regra do update_val).
  DbStatus delete_val(
    std::string_view name,
    const Record& where,
    size_t* affected = nullptr
  );
};
