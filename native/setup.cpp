#include "setup.h"
#include "utils.h"

#include <filesystem>
#include <iostream>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>


//retorna false apeanas se a instalação falhar
bool Setup::install()
{
  fs::path install_dir = get_install_dir(); 

  if(install_dir.empty())
  {
    std::cout << "Diretorio de instalacao nao foi encontrado. O app nao pode ser instalado." << std::endl;
    return false;
  }

  wchar_t buf[MAX_PATH];
  DWORD len = GetModuleFileNameW(nullptr, buf, MAX_PATH);
  if (len == 0 || len == MAX_PATH)
  {
    std::cout 
    << "Nao foi possivel descobrir o caminho ate o executavel. Instalacao abortada." 
    << std::endl;
    return false;
  }
  
  const fs::path dst = install_dir / EXE_NAME;
  const fs::path src = buf;
  std::error_code ec;

  if (fs::exists(dst, ec) && fs::equivalent(src, dst, ec)){
    std::cout << "App Ja esta Instalado. Nada a fazer." << std::endl;
    return true;
  }
  ec.clear();
  
  fs::create_directories(install_dir, ec);
  if (ec)
  {
    std::cout 
      << "Nao foi possivel criar o diretorio de instalacao. Instalacao abortada." << "\n"
      << "Error: " << ec << std::endl;
    return false;
  }

  fs::copy_file(src, dst, fs::copy_options::overwrite_existing, ec);
  if (ec)
  {
    std::cout 
      << "Nao foi possivel copiar o executável para o diretorio de instalação. Instalação abortada." << "\n"
      << "Error: " << ec << std::endl;
    return false;
  }

  // Registra para iniciar com o Windows (aspas protegem caminhos com espaço)
  std::wstring cmd = L"\"" + dst.wstring() + L"\"";
  LSTATUS r = RegSetKeyValueW(
    HKEY_CURRENT_USER, RUN_KEY, RUN_NAME, REG_SZ,
    cmd.c_str(),
    (DWORD)((cmd.size() + 1) * sizeof(wchar_t))
  );
  if(r != ERROR_SUCCESS)
  {
    std::cout 
    << "App instalado, mas nao foi possivel ativar a inicializacao aoutomatica." << "\n"
    << "Error: " << r << std::endl;
  }
  
  std::cout << "App Instalado" << std::endl;
  return true;
}

bool Setup::uninstall()
{
  fs::path install_dir = get_install_dir(); 

  if(install_dir.empty())
  {
    std::cout << "Diretorio de instalacao nao foi encontrado. O app nao pode ser desinstalado." << std::endl;
    return false;
  }

  LSTATUS r = RegDeleteKeyValueW(HKEY_CURRENT_USER, RUN_KEY, RUN_NAME);
  if (r != ERROR_SUCCESS && r != ERROR_FILE_NOT_FOUND)
  {
    std::cout << "Nao foi possivel remover a inicializacao automatica." << "\n"
    << "Error: " << r << std::endl;
    return false;
  }

  std::error_code ec;
  fs::remove_all(install_dir, ec);
  if (ec)
  {
    std::cout << "Nao foi possivel remover o diretorio de instalacao." << "\n"
    << "Error: " << ec << std::endl;
    return false;
  }
  return true;
}
