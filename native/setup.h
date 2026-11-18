#pragma once

class Setup
{
private:

public:
  static constexpr const wchar_t* RUN_KEY = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
  static constexpr const wchar_t* RUN_NAME = L"SecretariaVirtual";
  static constexpr const wchar_t* EXE_NAME = L"native_comp.exe";

  Setup() = default;

  bool install();
  bool uninstall();
};