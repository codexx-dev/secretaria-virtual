#include "app_config.h"
#include "platform/platform.h"
#include "platform/sys_tray.h"
#include "server_app.h"
#include "setup.h"

#include <iostream>

int main()
{
  Setup setup;
  setup.install();

  ServerApp server;
  if (!server.start())
  {
    std::cout << "Pressione Enter para encerrar.\n";
    std::cin.get();
    return 1;
  }

  std::cout << "Use o icone da bandeja para reiniciar ou encerrar." << std::endl;

  const SysTray::Action action = SysTray::run(AppConfig::get_instance().file());

  server.stop();   // libera a porta antes de a nova instancia subir
  if (action == SysTray::Action::Restart) start_new_instance();

  return 0;
}
