#include "server_app.h"
#include "server_config.h"
#include "server_routes.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace
{
  constexpr char IP[] = "127.0.0.1";

  // CORS: so paginas das origens permitidas podem chamar a API pelo navegador.
  // Requisicoes sem "Origin" (curl, o proprio app) nao vem de um site e passam.
  void allow_origins(httplib::Server& svr, std::vector<std::string> origins)
  {
    svr.set_pre_routing_handler([origins](const httplib::Request& req, httplib::Response& res)
    {
      if (!req.has_header("Origin")) return httplib::Server::HandlerResponse::Unhandled;

      const std::string origin = req.get_header_value("Origin");
      if (std::find(origins.begin(), origins.end(), origin) == origins.end())
      {
        res.status = 403;
        return httplib::Server::HandlerResponse::Handled;
      }

      res.set_header("Access-Control-Allow-Origin", origin);
      res.set_header("Vary", "Origin");

      if (req.method != "OPTIONS") return httplib::Server::HandlerResponse::Unhandled;

      // Preflight: o navegador pergunta antes de POST/PUT/DELETE com JSON.
      res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
      res.set_header("Access-Control-Allow-Headers", "Content-Type");
      if (req.has_header("Access-Control-Request-Private-Network"))   // site publico -> 127.0.0.1
        res.set_header("Access-Control-Allow-Private-Network", "true");
      res.status = 204;
      return httplib::Server::HandlerResponse::Handled;
    });
  }
}

bool ServerApp::start()
{
  const ServerConfig& config = ServerConfig::get_instance();

  if (config.data_dir().empty())
  {
    std::cout << "A pasta de Dados nao foi encontrado. O app nao pode ser iniciado." << std::endl;
    return false;
  }

  allow_origins(m_svr, config.origins());
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
