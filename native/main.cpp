// Prova de conceito: servidor local que escuta o navegador e salva dados em disco.
//
//   GET  /api/ping                  -> "pong"

//
// Os arquivos ficam na pasta "dados/", dentro do diretório de trabalho (a pasta de onde o
// programa foi aberto).

#include <string>
#include <filesystem>
#include <iostream>
#include <httplib.h>

namespace fs = std::filesystem;

namespace {

constexpr const char* IP = "127.0.0.1";
constexpr int DEFAULT_PORT = 8080;

// // ----------------------------------------------------------------------------
// // Utilidades
// // ----------------------------------------------------------------------------

// Writes to a temporary file and renames it: avoids leaving a half-written .txt if the PC shuts down.
bool write_atomic(const fs::path& path, const std::string& content) {
  const fs::path tmp = path.string() + ".tmp";
  {
    std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
    if (!out) return false;
    out << content;
    out.flush();
    if (!out) return false;
  }
  std::error_code ec;
  fs::rename(tmp, path, ec);
  return !ec;
}

}  // namespace

int main() {
  // Caminho relativo: a pasta "dados" fica no diretório de trabalho (de onde o programa foi aberto).
  const fs::path data_path = "dados";
  std::error_code err;
  fs::create_directories(data_path, err);
  if (err) {
    std::cerr << "Nao foi possivel criar a pasta de dados: " << err.message() << "\n";
    return 1;
  }
  
  // Origens que podem chamar a API: a própria página de teste e o Vite em modo dev.
  // Quando o frontend de produção tiver outro endereço, adicione aqui.
  const std::string p = std::to_string(DEFAULT_PORT);
  const std::set<std::string> origins_ok = {
      "http://127.0.0.1:" + p, "http://localhost:" + p,
      "http://127.0.0.1:5173", "http://localhost:5173",
  };

  httplib::Server svr;

  svr.Get("/api/ping", [](const httplib::Request &, httplib::Response &res) {
    res.set_content("pong", "text/plain");
  });

    if (!svr.bind_to_port(IP, DEFAULT_PORT)) {
        std::cerr << "Falha ao iniciar (porta " << DEFAULT_PORT << " ocupada?)\n";
        return 1;
    }
 
    // listen_after_bind() bloqueia, então roda numa thread e a principal
    // espera o usuário apertar Enter para encerrar.
    std::thread servidor([&] { svr.listen_after_bind(); });
 
    std::cout << "Servidor em " << IP << ":" << DEFAULT_PORT << "\n"
              << "Pressione Enter para encerrar.\n";
    std::cin.get();
 
    svr.stop();
    servidor.join();
    return 0;
}