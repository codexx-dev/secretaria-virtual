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

  // Host: so o proprio endereco do servidor. Barra o DNS rebinding, em que o
  // dominio de um site passa a apontar para 127.0.0.1 e a pagina dele le a API.
  bool is_allowed_host(const std::string& host, int port)
  {
    const std::string suffix = ":" + std::to_string(port);
    for (const char* name : {"127.0.0.1", "localhost"})
      if (host == name + suffix || (port == 80 && host == name)) return true;
    return false;
  }

  // Alteracao de dados so com corpo JSON. Formularios de outros sites nao
  // conseguem mandar esse tipo, e fetch/XHR com ele passam pelo preflight (CORS).
  bool needs_json(const httplib::Request& req)
  {
    return req.method == "POST" || req.method == "PUT" || req.method == "DELETE";
  }

  bool is_json(const httplib::Request& req)
  {
    return req.get_header_value("Content-Type").rfind("application/json", 0) == 0;
  }

  // Filtro que roda antes de todas as rotas: Host, origem (CORS) e tipo do corpo.
  // Requisicoes sem "Origin" (curl, o proprio app) nao vem de um site e passam no CORS.
  void filter_requests(httplib::Server& svr, int port)
  {
    svr.set_pre_routing_handler([port](const httplib::Request& req, httplib::Response& res)
    {
      if (!is_allowed_host(req.get_header_value("Host"), port))
      {
        res.status = 403;
        return httplib::Server::HandlerResponse::Handled;
      }

      if (req.has_header("Origin"))
      {
        const std::string origin = req.get_header_value("Origin");
        if (!is_allowed_origin(origin))
        {
          res.status = 403;
          return httplib::Server::HandlerResponse::Handled;
        }

        res.set_header("Access-Control-Allow-Origin", origin);
        res.set_header("Vary", "Origin");

        if (req.method == "OPTIONS")
        {
          // Preflight: o navegador pergunta antes de POST/PUT/DELETE com JSON.
          res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
          res.set_header("Access-Control-Allow-Headers", "Content-Type");
          if (req.has_header("Access-Control-Request-Private-Network"))   // site publico -> 127.0.0.1
            res.set_header("Access-Control-Allow-Private-Network", "true");
          res.status = 204;
          return httplib::Server::HandlerResponse::Handled;
        }
      }

      if (needs_json(req) && !is_json(req))
      {
        res.status = 415;
        res.set_content("{\"err\":\"Envie o corpo com Content-Type: application/json.\"}", "application/json");
        return httplib::Server::HandlerResponse::Handled;
      }

      return httplib::Server::HandlerResponse::Unhandled;
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

  filter_requests(m_svr, config.port());
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
