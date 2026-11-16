#include "utils.h"
#include <fstream>
#include <iostream>

namespace
{
  // Quebra de linha padrao do Windows/Excel.
  constexpr const char* EOL = "\r\n";

  // Remove o '\r' que sobra do std::getline em arquivos com CRLF.
  void strip_cr(std::string& s)
  {
    if (!s.empty() && s.back() == '\r') s.pop_back();
  }

  // Garante que a pasta do arquivo exista.
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
}  // namespace


bool write_byte(const fs::path& path, std::string_view& data)
{
  if (path.empty() || data.empty())
  {
    std::cout << "Caminho ou conteudo invalido. Escrita abortada." << std::endl;
    return false;
  }

  if (!ensure_parent_dir(path)) return false;

  fs::path tmp = path;
  tmp += L".tmp";

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

bool write_line(const fs::path& path, std::string_view& content, int line)
{
  if (path.empty() || content.empty())
  {
    std::cout << "Caminho ou conteudo invalido. Escrita abortada." << std::endl;
    return false;
  }

  if (line < 0) {
    if (!ensure_parent_dir(path)) return false;

    std::ofstream out(path, std::ios::app | std::ios::binary);
    if (!out)
    {
      std::cout << "Nao foi possivel abrir o arquivo para escrita." << std::endl;
      return false;
    }
    out << content << EOL;
    std::cout << "line < 0, conteudo escrito no final do arquivo." << std::endl;
    return out.good();
  }

  fs::path tmp = path;
  tmp += L".tmp";

  {
    std::ifstream in(path, std::ios::binary);
    std::ofstream out(tmp, std::ios::binary | std::ios::trunc);

    if (!in || !out)
    {
      std::cout << "Nao foi possivel abrir o arquivo para escrita." << std::endl;
      return false;
    }

    std::string buf;
    int i = 0;
    bool find = false;

    while (std::getline(in, buf))
    {
      strip_cr(buf);
      if (i == line)
      {
        out << content << EOL;
        find = true;
      }
      else out << buf << EOL;
      
      ++i;
    }

    out.flush();
    if (!find || !out.good()) {
      out.close();
      std::error_code ec;
      fs::remove(tmp, ec);
      std::cout << "Nao foi possivel encontrar a linha " << line << ". Operacao abordata." << std::endl;
      return false;
    }
  }

  std::error_code ec;
  fs::rename(tmp, path, ec);
  if(ec)
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

bool read_line(const fs::path& path, std::string& out, int line)
{
  if (line < 0)
  {
    std::cout << "Numero da linha invalida. Leitura Abortada." << std::endl;
    return false;
  }
  std::ifstream in(path, std::ios::binary);
  if (!in)
  {
    std::cout << "Nao foi possivel abrir o arquivo para leitura." << std::endl;
    return false;
  }

  std::string buf;
  int i = 0;
  while (std::getline(in, buf)) {
    if (i == line) {
      strip_cr(buf);
      out = buf;
      return true;
    }
    ++i;
  }

  std::cout << "A linha especificada nao existe." << std::endl;
  return false;   // a linha não existe
}