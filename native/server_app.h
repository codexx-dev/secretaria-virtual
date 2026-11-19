#pragma once

#include <httplib.h>
#include <thread>

class ServerApp
{
private:
  std::thread m_thread;
  httplib::Server m_svr;

public:
  ServerApp() = default;
  ~ServerApp() { stop(); }

  bool start();
  void stop();
};
