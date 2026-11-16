#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

// ----------------------------------------------------------------------------
// Arquivo com trava do Windows
// ----------------------------------------------------------------------------

enum class FileStatus
{
  Ok,
  NotFound,   // arquivo ou pasta nao existe
  Locked,     // em uso por outro programa (ex.: Excel) ou sem permissao
  IoError,    // outra falha de leitura/escrita
};

enum class OpenMode
{
  Existing,         // falha com NotFound se o arquivo nao existir
  CreateIfMissing,  // cria o arquivo (e as pastas) se nao existir
};

// Arquivo aberto com trava obrigatoria do Windows: enquanto estiver aberto,
// nenhum outro programa (Excel, Bloco de Notas, outro componente, outra
// maquina na rede) consegue gravar ou apagar o arquivo; so ler.
// A trava e liberada ao fechar (close() ou destrutor), inclusive se o
// processo cair.
class LockedFile
{
private:
  void* m_handle = nullptr;   // HANDLE (evita incluir windows.h aqui)
  bool m_created = false;

public:
  static constexpr std::size_t MAX_READ = 64u * 1024 * 1024;

  LockedFile() = default;
  ~LockedFile() { close(); }

  LockedFile(const LockedFile&) = delete;
  LockedFile& operator=(const LockedFile&) = delete;

  // Abre e trava o arquivo. Se outro programa estiver com ele aberto para
  // escrita, tenta de novo por ate timeout_ms milissegundos.
  FileStatus open(const fs::path& path, OpenMode mode, unsigned timeout_ms = 3000);
  void close();

  bool is_open() const { return m_handle != nullptr; }

  // true se open() acabou de criar o arquivo (CreateIfMissing).
  bool created() const { return m_created; }

  // Le o arquivo inteiro (no maximo max_size bytes).
  FileStatus read_all(std::string& out, std::size_t max_size = MAX_READ);

  // Substitui todo o conteudo do arquivo.
  FileStatus write_all(std::string_view data);

  // Apaga o arquivo e fecha.
  FileStatus remove();
};