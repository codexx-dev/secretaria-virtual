#include "setup.h"
#include "log.h"
#include "platform/platform.h"
#include "utils.h"

#include <filesystem>

// Retorna false apenas se a instalacao falhar.
bool Setup::install()
{
  const fs::path install_dir = get_install_dir();
  if (install_dir.empty())
  {
    log_error("Pasta de instalacao nao encontrada. O app nao pode ser instalado.");
    return false;
  }

  fs::path src;
  if (!get_executable_path(src))
  {
    log_error("Caminho do executavel nao encontrado. Instalacao abortada.");
    return false;
  }

  const fs::path dst = install_dir / executable_name();
  std::error_code ec;

  if (fs::exists(dst, ec) && fs::equivalent(src, dst, ec))
  {
    log_info("App ja instalado em " + quoted(install_dir) + ".");
    return true;
  }
  ec.clear();

  fs::create_directories(install_dir, ec);
  if (ec)
  {
    log_error("Falha ao criar a pasta de instalacao " + quoted(install_dir) + ": " + describe(ec));
    return false;
  }

  fs::copy_file(src, dst, fs::copy_options::overwrite_existing, ec);
  if (ec)
  {
    log_error("Falha ao copiar o executavel para " + quoted(dst) + ": " + describe(ec));
    return false;
  }

  long r = 0;
  if (!enable_autostart(dst, r))
    log_error("App instalado, mas a inicializacao automatica falhou: " + system_error_message(r));

  log_info("App instalado em " + quoted(install_dir) + ".");
  return true;
}

bool Setup::uninstall()
{
  const fs::path install_dir = get_install_dir();
  if (install_dir.empty())
  {
    log_error("Pasta de instalacao nao encontrada. O app nao pode ser desinstalado.");
    return false;
  }

  long r = 0;
  if (!disable_autostart(r))
  {
    log_error("Falha ao remover a inicializacao automatica: " + system_error_message(r));
    return false;
  }

  std::error_code ec;
  fs::remove_all(install_dir, ec);
  if (ec)
  {
    log_error("Falha ao remover a pasta de instalacao " + quoted(install_dir) + ": " + describe(ec));
    return false;
  }
  return true;
}
