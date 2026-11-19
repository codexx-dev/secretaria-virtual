#pragma once

#include <filesystem>
#include <mutex>
#include <string>

namespace fs = std::filesystem;

// Configuracoes do app, lidas de <instalacao>/config.json na primeira chamada
// de get_instance(). Se o arquivo nao existir (ou estiver vazio), e criado
// com os valores padrao. Pode ser lida e alterada de qualquer thread.
class AppConfig
{
private:
  mutable std::mutex m_mutex;
  fs::path m_file;
  fs::path m_data_dir;
  int m_port;
  std::string m_user;   // UTF-8

  AppConfig();
  void load();
  bool save() const;

public:
  static AppConfig& get_instance();

  AppConfig(const AppConfig&) = delete;
  AppConfig& operator=(const AppConfig&) = delete;

  const fs::path& file() const { return m_file; }   // nao muda depois de criada

  fs::path data_dir() const    { std::lock_guard<std::mutex> lock(m_mutex); return m_data_dir; }
  int port() const             { std::lock_guard<std::mutex> lock(m_mutex); return m_port; }
  std::string user() const     { std::lock_guard<std::mutex> lock(m_mutex); return m_user; }

  // Grava no arquivo e, se der certo, passa a valer na memoria.
  bool update(const fs::path& data_dir, int port, const std::string& user);
};
