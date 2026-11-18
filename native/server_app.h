#pragma once

#include <httplib.h>
#include <string>

class ServerApp
{
private:
  std::thread m_thread;
  httplib::Server m_svr;

inline std::string create_http_eror(const std::string& message){
  return "{\"err\":\""+ message +"\"}";
}
  

public:
  ServerApp () = default;
  ~ServerApp() { stop(); }

  bool start();
  void stop();
};
