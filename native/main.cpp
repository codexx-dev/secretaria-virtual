#include "log.h"
#include "platform/platform.h"
#include "platform/sys_tray.h"
#include "server_app.h"
#include "server_config.h"
#include "setup.h"

#ifdef LOG_TO_CONSOLE
#include <iostream>
#endif

int main()
{
  fs::path exe;
  get_executable_path(exe);
  log_info("Iniciado: versao " NAT_COMP_VERSION ", " + quoted(exe));

  Setup setup;
  setup.install();

  ServerApp server;
  if (!server.start())
  {
    log_error("Encerrado: o servidor nao iniciou.");
#ifdef LOG_TO_CONSOLE
    std::cout << "Pressione Enter para encerrar.\n";
    std::cin.get();
#endif
    return 1;
  }

  log_info("Usuario: " + ServerConfig::get_instance().user() + ". Use o icone da bandeja para reiniciar ou encerrar.");

  const SysTray::Action action = SysTray::run(ServerConfig::get_instance().file());

  server.stop();   // libera a porta antes de a nova instancia subir
  if (action == SysTray::Action::Restart)
  {
    log_info("Reiniciando.");
    if (!start_new_instance()) log_error("Falha ao iniciar a nova instancia.");
  }
  else
    log_info("Encerrado pelo usuario.");

  return 0;
}
