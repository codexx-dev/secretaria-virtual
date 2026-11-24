#include "utils.h"
#include "log.h"
#include "server_config.h"

#include <filesystem>
#include <fstream>
#include <string>

// ----------------------------------------------------------------------------
// Arquivos simples (sem trava)
// ----------------------------------------------------------------------------

bool ensure_parent_dir(const fs::path& path)
{
  const fs::path parent = path.parent_path();
  if (parent.empty()) return true;

  std::error_code ec;
  fs::create_directories(parent, ec);
  if (ec)
  {
    log_error("Falha ao criar a pasta " + quoted(parent) + ": " + describe(ec));
    return false;
  }
  return true;
}

bool write_byte(const fs::path& path, std::string_view data)
{
  if (path.empty() || data.empty())
  {
    log_error("Escrita abortada: caminho ou conteudo vazio (" + quoted(path) + ").");
    return false;
  }

  if (!ensure_parent_dir(path)) return false;

  fs::path tmp = path;
  tmp += ".tmp";

  {
    std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
    if (!out)
    {
      log_error("Falha ao abrir para escrita " + quoted(tmp) + ".");
      return false;
    }

    out.write(data.data(), data.size());
    out.flush();

    if (!out.good())
    {
      log_error("Falha ao escrever em " + quoted(tmp) + ".");
      out.close();
      std::error_code rm_ec;
      fs::remove(tmp, rm_ec);   // nao deixa lixo para tras
      return false;
    }
  }

  std::error_code ec;
  fs::rename(tmp, path, ec);
  if (ec)
  {
    log_error("Falha ao substituir " + quoted(path) + ": " + describe(ec));
    std::error_code rm_ec;
    fs::remove(tmp, rm_ec);
    return false;
  }

  return true;
}

bool erase(const fs::path& path)
{
  if (path.empty())
  {
    log_error("Exclusao abortada: caminho vazio.");
    return false;
  }

  std::error_code ec;
  fs::remove(path, ec);
  if (ec)
  {
    log_error("Falha ao apagar " + quoted(path) + ": " + describe(ec));
    return false;
  }
  return true;
}

bool read_bytes(const fs::path& path, std::vector<char>& out)
{
  std::ifstream in(path, std::ios::binary | std::ios::ate);
  if (!in)
  {
    log_error("Falha ao abrir para leitura " + quoted(path) + ".");
    return false;
  }

  const std::streamsize size = in.tellg();   // ate: ja abre no fim
  in.seekg(0, std::ios::beg);

  out.resize(size > 0 ? static_cast<size_t>(size) : 0);
  if (size < 0 || (size > 0 && !in.read(out.data(), size)))
  {
    log_error("Falha ao ler " + quoted(path) + ".");
    out.clear();
    return false;
  }
  return true;
}

// ----------------------------------------------------------------------------
// Caminhos dentro da pasta de dados
// ----------------------------------------------------------------------------

bool is_inside_data_dir(const fs::path& path)
{
  const fs::path data_dir = ServerConfig::get_instance().data_dir();
  if (data_dir.empty() || path.empty()) return false;

  std::error_code ec;
  const fs::path base = fs::weakly_canonical(data_dir, ec);
  if (ec) return false;
  const fs::path target = fs::weakly_canonical(path, ec);
  if (ec) return false;

  // Caminho do alvo a partir da base: se comeca com "..", esta fora.
  const fs::path rel = target.lexically_relative(base);
  return !rel.empty() && rel != "." && *rel.begin() != "..";
}
