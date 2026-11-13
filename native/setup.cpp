#include "setup.h"
#include <filesystem>
#include <iostream>
#include <windows.h>

Setup::Setup()
{
  const wchar_t* appdata = _wgetenv(L"APPDATA");
  if (!appdata)
  {
    std::cout << "APPDATA nao foi encontrado. O app nao pode ser instalado." << std::endl;
    return;
  }
  m_install_dir = fs::path(appdata) / L"SecretariaVirtual";
}

//retorna false apeanas se a instalação falhar
bool Setup::install()
{
  if(m_install_dir.empty())
  {
    std::cout << "APPDATA nao foi encontrado. O app nao pode ser instalado." << std::endl;
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
  
  const fs::path dst = m_install_dir / EXE_NAME;
  const fs::path src = buf;
  std::error_code ec;

  if (fs::exists(dst, ec) && fs::equivalent(src, dst, ec)){
    std::cout << "App Ja esta Instalado. Nada a fazer." << std::endl;
    return true;
  }
  ec.clear();
  
  fs::create_directories(m_install_dir, ec);
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
  if (m_install_dir.empty()) return false;

  LSTATUS r = RegDeleteKeyValueW(HKEY_CURRENT_USER, RUN_KEY, RUN_NAME);
  if (r != ERROR_SUCCESS && r != ERROR_FILE_NOT_FOUND)
  {
    std::cout << "Nao foi possivel remover a inicializacao automatica." << "\n"
    << "Error: " << r << std::endl;
    return false;
  }

  std::error_code ec;
  fs::remove_all(m_install_dir, ec);
  if (ec)
  {
    std::cout << "Nao foi possivel remover o diretorio de instalacao." << "\n"
    << "Error: " << ec << std::endl;
    return false;
  }
  return true;
}
