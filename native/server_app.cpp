#include "server_app.h"
#include "app_config.h"
#include "server_routes.h"

#include <iostream>

namespace
{
  constexpr char IP[] = "127.0.0.1";
}

bool ServerApp::start()
{
  const AppConfig& config = AppConfig::get_instance();

  if (config.data_dir().empty())
  {
    std::cout << "A pasta de Dados nao foi encontrado. O app nao pode ser iniciado." << std::endl;
    return false;
  }

  register_routes(m_svr);

  if (!m_svr.bind_to_port(IP, config.port()))
  {
    std::cout << "Porta ocupada. O app nao pode ser iniciado." << std::endl;
    return false;
  }
  m_thread = std::thread([this] { m_svr.listen_after_bind(); });

  std::cout << "Servidor em http://" << IP << ":" << config.port() << std::endl;
  return true;
}

void ServerApp::stop()
{
  m_svr.stop();
  if (m_thread.joinable()) m_thread.join();
}
