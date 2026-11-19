#include "server_app.h"
#include "app_config.h"
#include "utils.h"

#include <filesystem>
#include <rapidjson/document.h>

namespace json = rapidjson;
namespace
{
  inline constexpr char IP[] = "127.0.0.1";
}  // namespace

bool ServerApp::start()
{
  const AppConfig& config = AppConfig::get_instance();

  if (config.data_dir().empty())
  {
    std::cout << "A pasta de Dados nao foi encontrado. O app nao pode ser iniciado." << std::endl;
    return false;
  }

  ////////////////////
  // configura routes
  ////////////////////

  m_svr.Get("/api/ping", [](const httplib::Request &, httplib::Response &res) 
  {
    res.set_content("pong", "text/plain"); //test
  });

  /////////
  // Dados
  /////////

  m_svr.Get( "/api/dados", [this](const httplib::Request &req, httplib::Response &res)
  {
    json::Document req_body;
    req_body.Parse(req.body);

    fs::path data_src(req_body["src"].GetString());
    std::vector<char> data_arr;
    if(!read_bytes(data_src, data_arr))
    {
      auto err = create_http_eror("Não foi possivel acessar o caminho especificado.");
      res.set_content(err, "application/json");
      res.status = 500;
    }

    res.status = 501;   // nao implementado
  });

  m_svr.Post( "/api/dados", [this](const httplib::Request &req, httplib::Response &res)
  {
    res.status = 501;   // nao implementado
  });

  m_svr.Delete( "/api/dados", [this](const httplib::Request &req, httplib::Response &res)
  {
    res.status = 501;   // nao implementado
  });

  /////////
  // Configurações
  /////////

  m_svr.Get( "/api/configuracao", [this](const httplib::Request &req, httplib::Response &res)
  {
    res.status = 501;   // nao implementado
  });

  m_svr.Post( "/api/configuracao", [this](const httplib::Request &req, httplib::Response &res)
  {
    res.status = 501;   // nao implementado
  });

  //
  ////////

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