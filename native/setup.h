#pragma once
#include <filesystem>

namespace fs = std::filesystem;

class Setup
{
private:
  fs::path m_install_dir;

public:
  static constexpr const wchar_t* RUN_KEY = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
  static constexpr const wchar_t* RUN_NAME = L"SecretariaVirtual";
  static constexpr const wchar_t* EXE_NAME = L"native_comp.exe";

  Setup();

  inline const wchar_t* get_install_dir() { return m_install_dir.c_str(); }

  bool install();
  bool uninstall();
};