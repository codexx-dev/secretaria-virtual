#pragma once

#include <httplib.h>
#include <filesystem>

namespace fs = std::filesystem;

class ServerApp
{
private:
  std::string m_ip;
  int m_port;

  std::thread m_thread;
  httplib::Server m_svr;
  fs::path m_data_dir;

public:
  ServerApp (const char* ip, int port): m_ip(ip), m_port(port) { }

  bool start();
  void stop();
};
