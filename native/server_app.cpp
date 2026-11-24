#include "server_app.h"
#include "log.h"
#include "server_config.h"
#include "server_routes.h"

#include <algorithm>
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

  // Uma linha por resposta: "DELETE /api/dados src=T/ALUNOS -> 404 {"err":"..."}".
  // O corpo so entra em erro, onde ele e a descricao enviada ao cliente.
  void log_responses(httplib::Server& svr)
  {
    svr.set_logger([](const httplib::Request& req, const httplib::Response& res)
    {
      std::string line = req.method + " " + req.path;
      if (req.has_param("src")) line += " src=" + req.get_param_value("src");
      line += " -> " + std::to_string(res.status);

      if (res.status < 400) return log_info(line);
      if (!res.body.empty()) line += " " + res.body;
      log_error(line);
    });
  }
}

bool ServerApp::start()
{
  const ServerConfig& config = ServerConfig::get_instance();

  if (config.data_dir().empty())
  {
    log_error("Pasta de dados nao definida. O servidor nao pode ser iniciado.");
    return false;
  }

  allow_origins(m_svr, config.origins());
  log_responses(m_svr);
  register_routes(m_svr);

  if (!m_svr.bind_to_port(IP, config.port()))
  {
    log_error("Porta " + std::to_string(config.port()) + " ocupada. O servidor nao pode ser iniciado.");
    return false;
  }
  m_thread = std::thread([this] { m_svr.listen_after_bind(); });

  log_info("Servidor em http://" + std::string(IP) + ":" + std::to_string(config.port()));
  return true;
}

void ServerApp::stop()
{
  m_svr.stop();
  if (m_thread.joinable()) m_thread.join();
}
