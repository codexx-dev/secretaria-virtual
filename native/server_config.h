#pragma once

#include <filesystem>
#include <mutex>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// Configuracoes do servidor, em <instalacao>/config.json. Toda leitura,
// validacao e gravacao do arquivo passa por aqui.
//
// Na primeira chamada de get_instance() o arquivo e lido. Se nao existir, ou
// se alguma opcao faltar ou for invalida, ele e regravado com as opcoes
// validas e o padrao no lugar das demais. Pode ser usada de qualquer thread.
class ServerConfig
{
public:
  enum class Error { None, InvalidDataDir, InvalidPort, InvalidUser, SaveFailed };

private:
  mutable std::mutex m_mutex;
  fs::path m_file;
  fs::path m_data_dir;
  int m_port;
  std::string m_user;                   // UTF-8
  std::vector<std::string> m_origins;   // ex.: "https://site.com"

  ServerConfig();
  bool load();
  bool save() const;

public:
  static ServerConfig& get_instance();

  ServerConfig(const ServerConfig&) = delete;
  ServerConfig& operator=(const ServerConfig&) = delete;

  const fs::path& file() const { return m_file; }   // nao muda depois de criada

  fs::path data_dir() const                { std::lock_guard<std::mutex> lock(m_mutex); return m_data_dir; }
  int port() const                         { std::lock_guard<std::mutex> lock(m_mutex); return m_port; }
  std::string user() const                 { std::lock_guard<std::mutex> lock(m_mutex); return m_user; }
  std::vector<std::string> origins() const { std::lock_guard<std::mutex> lock(m_mutex); return m_origins; }

  // Valida, grava no arquivo e, se der certo, passa a valer na memoria.
  // A pasta de dados precisa existir, ficar na pasta do usuario ou na rede e
  // permitir leitura e escrita. A porta nova so vale apos reiniciar.
  Error update(const fs::path& data_dir, int port, const std::string& user);

  static const char* error_message(Error e);
};
