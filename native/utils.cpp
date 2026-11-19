#include "utils.h"
#include "app_config.h"

#include <filesystem>
#include <fstream>
#include <iostream>
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
    std::cout
    << "Nao foi possivel criar o diretorio do arquivo." << "\n"
    << "Error: " << ec << std::endl;
    return false;
  }
  return true;
}

bool write_byte(const fs::path& path, std::string_view data)
{
  if (path.empty() || data.empty())
  {
    std::cout << "Caminho ou conteudo invalido. Escrita abortada." << std::endl;
    return false;
  }

  if (!ensure_parent_dir(path)) return false;

  fs::path tmp = path;
  tmp += ".tmp";

  {
    std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
    if (!out)
    {
      std::cout << "Nao foi possivel abrir o arquivo temporario para escrita." << std::endl;
      return false;
    }

    out.write(data.data(), data.size());
    out.flush();

    if (!out.good())
    {
      std::cout << "Falha ao escrever no arquivo temporario." << std::endl;
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
    std::cout
    << "Nao foi possivel substituir o arquivo de destino." << "\n"
    << "Error: " << ec << std::endl;
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
    std::cout << "Caminho invalido. Exclusao abortada." << std::endl;
    return false;
  }

  std::error_code ec;
  fs::remove(path, ec);
  if (ec)
  {
    std::cout
    << "Nao foi possivel apagar o arquivo." << "\n"
    << "Error: " << ec << std::endl;
    return false;
  }
  return true;
}

bool read_bytes(const fs::path& path, std::vector<char>& out)
{
  std::ifstream in(fs::path(path), std::ios::binary | std::ios::ate);
  if (!in)
  {
    std::cout << "Nao foi possivel ler o arquivo no caminho especificado." << std::endl;
    return false;
  }

  std::streamsize size = in.tellg();   // ate: já abre no fim
  if (size < 0)
  {
    std::cout << "Falha ao ler o arquivo." << std::endl;
    return false;
  }

  in.seekg(0, std::ios::beg);

  out.resize(static_cast<size_t>(size));
  if (size > 0 && !in.read(out.data(), size)) {
    std::cout << "Falha ao ler o arquivo." << std::endl;
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
  const fs::path& data_dir = AppConfig::get_instance().data_dir();
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
