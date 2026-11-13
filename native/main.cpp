#include "server_app.h"
#include "setup.h"
#include <iostream>


namespace 
{
  constexpr const char* IP = "127.0.0.1";
  constexpr int DEFAULT_PORT = 8080;
}  // namespace


int main() {
  Setup setup;

  setup.install();
  std::cout << "Pressione Enter para encerrar.\n";
  std::cin.get();
  
  // Origens que podem chamar a API: a própria página de teste e o Vite em modo dev.
  // Quando o frontend de produção tiver outro endereço, adicione aqui.
  //const std::string p = std::to_string(DEFAULT_PORT);
  //const std::set<std::string> origins_ok = {
  //    "http://127.0.0.1:" + p, "http://localhost:" + p,
  //    "http://127.0.0.1:5173", "http://localhost:5173",
  //};


  // ServerApp server(IP, DEFAULT_PORT);


  // if(!server.start()){
  //   std::cout << "Erro ao iniciar o Servidor" << "\n"
  //           << "Pressione Enter para encerrar.\n";
  //   std::cin.get();
  //   return 1;
  // }
 
  // std::cout << "Servidor em " << IP << ":" << DEFAULT_PORT << "\n"
  //           << "Pressione Enter para encerrar.\n";
  // std::cin.get();
  
  // server.stop();

  return 0;
}