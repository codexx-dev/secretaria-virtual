#include "platform/platform.h"

#include <climits>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

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
