#include "server_app.h"
#include "utils.h"
#include <filesystem>
#include <string_view>


bool ServerApp::start()
{
  //obter caminho do config
  m_data_dir = fs::path("C::\\\\Secretaria Vistual"); //test

  //configura routes
  m_svr.Get("/ping", [](const httplib::Request &, httplib::Response &res) 
  {
    res.set_content("pong", "text/plain");
  });

  m_svr.Post( "/save-img", [this](const httplib::Request &req, httplib::Response &res)
  {
    //path form req.body
    fs::path path = m_data_dir / L"teste";
    //data from req.body
    std::string_view data;

    //checar se o caminho é valido
    //if()
    //salvar arquivo
    write_byte(path, data);
  });

  m_svr.Post( "/save-info", [this](const httplib::Request &req, httplib::Response &res)
  {
    //path form req.body
    fs::path path = m_data_dir / L"teste";
    //data from req.body
    std::string_view data;

    //checar se o caminho é valido
    //if()
    //salvar arquivo
    write_line(path, data);
  });

  ////

  if (!m_svr.bind_to_port(m_ip, m_port)) return false;
  // porta ocupada, IP inválido etc.

  m_thread = std::thread([this] { m_svr.listen_after_bind(); });

  return true;
}

void ServerApp::stop()
{
  m_svr.stop();
  if (m_thread.joinable()) m_thread.join();
}