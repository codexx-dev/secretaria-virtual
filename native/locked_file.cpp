#include "locked_file.h"
#include "utils.h"

#include <algorithm>
#include <chrono>
#include <thread>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace
{
  constexpr auto LOCK_RETRY = std::chrono::milliseconds(50);
  constexpr DWORD IO_CHUNK = 1u << 20;

  FileStatus status_from_error(DWORD err)
  {
    switch (err)
    {
      case ERROR_FILE_NOT_FOUND:
      case ERROR_PATH_NOT_FOUND:
        return FileStatus::NotFound;

      case ERROR_SHARING_VIOLATION:   // aberto por outro programa
      case ERROR_LOCK_VIOLATION:
      case ERROR_ACCESS_DENIED:       // sem permissao ou sendo apagado
        return FileStatus::Locked;

      default:
        return FileStatus::IoError;
    }
  }
}  // namespace

FileStatus LockedFile::open(const fs::path& path, OpenMode mode, unsigned timeout_ms)
{
  close();

  const bool create = mode == OpenMode::CreateIfMissing;
  if (create && !ensure_parent_dir(path)) return FileStatus::IoError;

  const auto deadline =
    std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);

  for (;;)
  {
    // FILE_SHARE_READ e a trava: os outros podem ler, mas nao gravar nem
    // apagar enquanto este handle estiver aberto. E a abertura falha se
    // outro programa ja estiver com o arquivo aberto para escrita.
    HANDLE h = CreateFileW(
      path.c_str(), GENERIC_READ | GENERIC_WRITE | DELETE, FILE_SHARE_READ,
      nullptr, create ? OPEN_ALWAYS : OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (h != INVALID_HANDLE_VALUE)
    {
      // Com OPEN_ALWAYS, ERROR_ALREADY_EXISTS indica que o arquivo ja existia.
      m_created = create && GetLastError() != ERROR_ALREADY_EXISTS;
      m_handle = h;
      return FileStatus::Ok;
    }

    const FileStatus st = status_from_error(GetLastError());
    if (st != FileStatus::Locked || std::chrono::steady_clock::now() >= deadline)
      return st;

    std::this_thread::sleep_for(LOCK_RETRY);
  }
}

void LockedFile::close()
{
  if (m_handle) CloseHandle(static_cast<HANDLE>(m_handle));
  m_handle = nullptr;
  m_created = false;
}

FileStatus LockedFile::read_all(std::string& out, std::size_t max_size)
{
  out.clear();
  if (!m_handle) return FileStatus::IoError;
  HANDLE h = static_cast<HANDLE>(m_handle);

  LARGE_INTEGER size;
  LARGE_INTEGER zero{};
  if (!GetFileSizeEx(h, &size) || !SetFilePointerEx(h, zero, nullptr, FILE_BEGIN))
    return status_from_error(GetLastError());
  if (static_cast<unsigned long long>(size.QuadPart) > max_size) return FileStatus::IoError;

  out.resize(static_cast<std::size_t>(size.QuadPart));
  std::size_t done = 0;
  while (done < out.size())
  {
    DWORD got = 0;
    const DWORD want = static_cast<DWORD>(std::min<std::size_t>(out.size() - done, IO_CHUNK));
    if (!ReadFile(h, out.data() + done, want, &got, nullptr))
    {
      const DWORD err = GetLastError();
      out.clear();
      return status_from_error(err);
    }
    if (got == 0) break;
    done += got;
  }
  out.resize(done);
  return FileStatus::Ok;
}

FileStatus LockedFile::write_all(std::string_view data)
{
  if (!m_handle) return FileStatus::IoError;
  HANDLE h = static_cast<HANDLE>(m_handle);

  LARGE_INTEGER zero{};
  if (!SetFilePointerEx(h, zero, nullptr, FILE_BEGIN))
    return status_from_error(GetLastError());

  std::size_t done = 0;
  while (done < data.size())
  {
    DWORD put = 0;
    const DWORD want = static_cast<DWORD>(std::min<std::size_t>(data.size() - done, IO_CHUNK));
    if (!WriteFile(h, data.data() + done, want, &put, nullptr) || put == 0)
      return status_from_error(GetLastError());
    done += put;
  }

  // Corta o que sobrou do conteudo antigo e manda tudo para o disco.
  if (!SetEndOfFile(h) || !FlushFileBuffers(h))
    return status_from_error(GetLastError());

  return FileStatus::Ok;
}

FileStatus LockedFile::remove()
{
  if (!m_handle) return FileStatus::IoError;

  // Marca para apagar; o Windows apaga ao fechar o handle.
  FILE_DISPOSITION_INFO info{};
  info.DeleteFile = TRUE;
  const BOOL ok = SetFileInformationByHandle(
    static_cast<HANDLE>(m_handle), FileDispositionInfo, &info, sizeof(info));
  const DWORD err = GetLastError();

  close();
  return ok ? FileStatus::Ok : status_from_error(err);
}
