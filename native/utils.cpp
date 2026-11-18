#include "utils.h"

#include <climits>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>


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
// Conversao de texto
// ----------------------------------------------------------------------------

bool utf8_to_wide(std::string_view in, std::wstring& out)
{
  out.clear();
  if (in.empty()) return true;
  if (in.size() > INT_MAX) return false;

  const int len = static_cast<int>(in.size());
  const int n = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, in.data(), len, nullptr, 0);
  if (n <= 0) return false;

  out.resize(static_cast<size_t>(n));
  return MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, in.data(), len, out.data(), n) == n;
}

bool wide_to_utf8(std::wstring_view in, std::string& out)
{
  out.clear();
  if (in.empty()) return true;
  if (in.size() > INT_MAX) return false;

  const int len = static_cast<int>(in.size());
  const int n = WideCharToMultiByte(CP_UTF8, 0, in.data(), len, nullptr, 0, nullptr, nullptr);
  if (n <= 0) return false;

  out.resize(static_cast<size_t>(n));
  return WideCharToMultiByte(CP_UTF8, 0, in.data(), len, out.data(), n, nullptr, nullptr) == n;
}

bool codepage_to_utf8(std::string_view in, unsigned codepage, std::string& out)
{
  out.clear();
  if (in.empty()) return true;
  if (in.size() > INT_MAX) return false;

  const int len = static_cast<int>(in.size());
  const int n = MultiByteToWideChar(codepage, 0, in.data(), len, nullptr, 0);
  if (n <= 0) return false;

  std::wstring wide(static_cast<size_t>(n), L'\0');
  if (MultiByteToWideChar(codepage, 0, in.data(), len, wide.data(), n) != n) return false;

  return wide_to_utf8(wide, out);
}

//
//
//
fs::path get_install_dir(){
  const wchar_t* appdata = _wgetenv(L"APPDATA");
  if(!appdata) return fs::path();
  return fs::path(appdata) / "SecretariaVirtual";
}

fs::path get_data_dir(){
  const wchar_t* user_prof = _wgetenv(L"USERPROFILE");
  if(!user_prof) return fs::path();
  return fs::path(user_prof) / "Documents" / "SecretariaVirtual";
}