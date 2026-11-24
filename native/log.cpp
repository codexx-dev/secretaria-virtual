#include "log.h"
#include "platform/platform.h"

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>

namespace
{
  std::mutex g_mutex;

  void write(const char* level, const std::string& message)
  {
    const std::time_t now = std::time(nullptr);
    std::tm local{};
    localtime_s(&local, &now);

    std::ostringstream line;
    line << std::put_time(&local, "%Y-%m-%d %H:%M:%S") << " [" << level << "] " << message << "\n";

    std::lock_guard<std::mutex> lock(g_mutex);

#ifdef LOG_TO_CONSOLE
    std::cout << line.str() << std::flush;
#endif

    const fs::path dir = get_install_dir();
    if (dir.empty()) return;

    std::error_code ec;
    fs::create_directories(dir, ec);
    std::ofstream(dir / "log.txt", std::ios::app) << line.str();
  }
}  // namespace

void log_info(const std::string& message)  { write("INFO", message); }
void log_error(const std::string& message) { write("ERRO", message); }

std::string quoted(const fs::path& path)
{
  return "\"" + fs::path(path).make_preferred().u8string() + "\"";   // barras padronizadas
}

std::string describe(const std::error_code& ec)
{
  // Codigo do Windows -> mensagem do sistema em UTF-8; os demais (errno) ja vem em ASCII.
  if (ec.category() == std::system_category()) return system_error_message(ec.value());
  return ec.message();
}
