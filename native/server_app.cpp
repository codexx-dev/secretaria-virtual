#include "server_app.h"
#include "log.h"
#include "server_config.h"
#include "server_routes.h"

#include <algorithm>
#include <string>
#include <vector>

// Origens de native/origins.txt, definidas pelo CMake (-DNAT_COMP_ORIGINS=...).
#ifndef NAT_COMP_ORIGINS
#error "NAT_COMP_ORIGINS nao definido: compile pelo CMake, que le native/origins.txt."
#endif

namespace
{
  constexpr char IP[] = "127.0.0.1";

  // Origens do build (origins.txt) ou pagina na propria maquina, em qualquer
  // porta: http(s)://localhost e http(s)://127.0.0.1.
  bool is_allowed_origin(const std::string& origin)
  {
    static const std::vector<std::string> build_origins{ NAT_COMP_ORIGINS };
    if (std::find(build_origins.begin(), build_origins.end(), origin) != build_origins.end()) return true;

    for (const char* local : {"http://localhost", "https://localhost", "http://127.0.0.1", "https://127.0.0.1"})
    {
      const std::string prefix = local;
      if (origin.rfind(prefix, 0) != 0) continue;
      const std::string port = origin.substr(prefix.size());   // "" ou ":5173"
      if (port.empty() || (port.size() > 1 && port[0] == ':' && port.find_first_not_of("0123456789", 1) == std::string::npos))
        return true;
    }
    return false;
  }

  // CORS: so paginas das origens permitidas podem chamar a API pelo navegador.
  // Requisicoes sem "Origin" (curl, o proprio app) nao vem de um site e passam.
  void allow_origins(httplib::Server& svr)
  {
    svr.set_pre_routing_handler([](const httplib::Request& req, httplib::Response& res)
    {
      if (!req.has_header("Origin")) return httplib::Server::HandlerResponse::Unhandled;

      const std::string origin = req.get_header_value("Origin");
      if (!is_allowed_origin(origin))
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

  allow_origins(m_svr);
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
