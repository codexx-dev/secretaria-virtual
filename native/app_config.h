#pragma once

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// Configuracoes do app, lidas de <instalacao>/config.json na primeira chamada
// de get_instance(). Se o arquivo nao existir (ou estiver vazio), e criado
// com os valores padrao. Depois de carregada, so leitura.
class AppConfig
{
private:
  fs::path m_file;
  fs::path m_data_dir;
  int m_port;
  std::string m_user;   // UTF-8

  AppConfig();
  void load();
  void save() const;

public:
  static AppConfig& get_instance();

  AppConfig(const AppConfig&) = delete;
  AppConfig& operator=(const AppConfig&) = delete;

  const fs::path& file() const { return m_file; }
  const fs::path& data_dir() const { return m_data_dir; }
  int port() const { return m_port; }
  const std::string& user() const { return m_user; }
};
