#include "app_config.h"
#include "platform/platform.h"
#include "utils.h"

#include <iostream>
#include <string_view>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

namespace json = rapidjson;

namespace
{
  constexpr int DEFAULT_PORT = 8080;
  constexpr const char* KEY_DATA_DIR = "dados";
  constexpr const char* KEY_PORT = "porta";

  constexpr const char* DEFAULT_USER = "default";
  constexpr const char* KEY_USER = "usuario";
}

AppConfig& AppConfig::get_instance()
{
  static AppConfig instance;
  return instance;
}

AppConfig::AppConfig()
  : m_data_dir(get_default_data_dir()), m_port(DEFAULT_PORT), m_user(DEFAULT_USER)
{
  const fs::path install_dir = get_install_dir();
  if (install_dir.empty())
  {
    std::cout << "Pasta de instalacao nao encontrada. Usando a configuracao padrao." << std::endl;
    return;
  }
  m_file = install_dir / "config.json";

  std::error_code ec;
  if (fs::exists(m_file, ec) && fs::file_size(m_file, ec) > 0)
    load();
  else
    save();
}

void AppConfig::load()
{
  std::vector<char> bytes;
  if (!read_bytes(m_file, bytes)) return;

  std::string_view text(bytes.data(), bytes.size());
  if (text.substr(0, 3) == "\xEF\xBB\xBF") text.remove_prefix(3);   // BOM do Bloco de Notas

  json::Document doc;
  doc.Parse(text.data(), text.size());
  if (doc.HasParseError() || !doc.IsObject())
  {
    std::cout << "Arquivo de configuracao invalido (nos caminhos, use \\\\ ou /). "
                 "Usando a configuracao padrao." << std::endl;
    return;
  }

  if (doc.HasMember(KEY_DATA_DIR) && doc[KEY_DATA_DIR].IsString())
    m_data_dir = fs::u8path(doc[KEY_DATA_DIR].GetString());

  if (doc.HasMember(KEY_PORT) && doc[KEY_PORT].IsInt())
  {
    const int port = doc[KEY_PORT].GetInt();
    if (port > 0 && port <= 65535) m_port = port;
  }

  if (doc.HasMember(KEY_USER) && doc[KEY_USER].IsString() && doc[KEY_USER].GetStringLength() > 0)
    m_user = doc[KEY_USER].GetString();
}

void AppConfig::save() const
{
  json::StringBuffer buf;
  json::PrettyWriter<json::StringBuffer> writer(buf);
  writer.StartObject();
  writer.Key(KEY_DATA_DIR);
  writer.String(m_data_dir.u8string());
  writer.Key(KEY_PORT);
  writer.Int(m_port);
  writer.Key(KEY_USER);
  writer.String(m_user);
  writer.EndObject();

  write_byte(m_file, std::string_view(buf.GetString(), buf.GetSize()));
}
