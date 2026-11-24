#include "platform/platform.h"

#include <cstdlib>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace
{
  constexpr const wchar_t* RUN_KEY = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
  constexpr const wchar_t* RUN_NAME = L"SecretariaVirtual";
}

fs::path get_install_dir()
{
  const wchar_t* appdata = _wgetenv(L"APPDATA");
  if (!appdata) return fs::path();
  return fs::path(appdata) / "SecretariaVirtual";
}

fs::path get_user_dir()
{
  const wchar_t* user_prof = _wgetenv(L"USERPROFILE");
  return user_prof ? fs::path(user_prof) : fs::path();
}

fs::path get_default_data_dir()
{
  const fs::path user_dir = get_user_dir();
  if (user_dir.empty()) return fs::path();
  return user_dir / "Documents" / "SecretariaVirtual";
}

bool is_network_path(const fs::path& path)
{
  std::error_code ec;
  const std::wstring root = fs::absolute(path, ec).root_name().wstring();   // "C:" ou "\\servidor\pasta"
  if (ec) return false;

  // "\\?\" e "\\.\" sao prefixos de caminho local, nao de rede.
  if (root.rfind(L"\\\\?\\", 0) == 0 || root.rfind(L"\\\\.\\", 0) == 0) return false;
  if (root.rfind(L"\\\\", 0) == 0) return true;   // \\servidor\pasta

  // Unidade mapeada (ex.: Z:).
  return root.size() == 2 && root[1] == L':' && GetDriveTypeW((root + L"\\").c_str()) == DRIVE_REMOTE;
}

bool get_executable_path(fs::path& out)
{
  wchar_t buf[MAX_PATH];
  const DWORD len = GetModuleFileNameW(nullptr, buf, MAX_PATH);
  if (len == 0 || len == MAX_PATH) return false;
  out = buf;
  return true;
}

const fs::path& executable_name()
{
  static const fs::path name = L"native_comp.exe";
  return name;
}

bool enable_autostart(const fs::path& exe, long& error)
{
  // aspas protegem caminhos com espaco
  const std::wstring cmd = L"\"" + exe.wstring() + L"\"";
  const LSTATUS r = RegSetKeyValueW(
    HKEY_CURRENT_USER, RUN_KEY, RUN_NAME, REG_SZ,
    cmd.c_str(),
    static_cast<DWORD>((cmd.size() + 1) * sizeof(wchar_t))
  );
  error = r;
  return r == ERROR_SUCCESS;
}

bool disable_autostart(long& error)
{
  const LSTATUS r = RegDeleteKeyValueW(HKEY_CURRENT_USER, RUN_KEY, RUN_NAME);
  error = r;
  return r == ERROR_SUCCESS || r == ERROR_FILE_NOT_FOUND;
}

bool start_new_instance()
{
  fs::path exe;
  if (!get_executable_path(exe)) return false;

  std::wstring cmd = L"\"" + exe.wstring() + L"\"";   // CreateProcessW pode alterar a string
  STARTUPINFOW si{};
  si.cb = sizeof(si);
  PROCESS_INFORMATION pi{};
  if (!CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi))
    return false;

  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);
  return true;
}

std::string system_error_message(long code)
{
  wchar_t* buf = nullptr;
  const DWORD len = FormatMessageW(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    nullptr, static_cast<DWORD>(code), 0, reinterpret_cast<wchar_t*>(&buf), 0, nullptr);

  std::string out;
  if (len > 0) wide_to_utf8(std::wstring_view(buf, len), out);
  if (buf) LocalFree(buf);

  while (!out.empty() && (out.back() == '\n' || out.back() == '\r')) out.pop_back();
  return out.empty() ? "erro " + std::to_string(code) : out;
}
