#include "server_config.h"
#include "log.h"
#include "platform/platform.h"
#include "utils.h"

#include <string_view>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

namespace json = rapidjson;

namespace
{
  constexpr const char* KEY_DATA_DIR = "dados";
  constexpr const char* KEY_PORT     = "porta";
  constexpr const char* KEY_USER     = "usuario";
  constexpr const char* KEY_ORIGINS  = "origins";

  constexpr int DEFAULT_PORT = 8080;
  constexpr const char* DEFAULT_USER = "default";
  constexpr const char* DEFAULT_ORIGIN = "https://codexx-dev.github.io";

  bool is_valid_port(int port) { return port > 0 && port <= 65535; }

  // Uma origem e so esquema, host e porta: "https://site.com/app/" -> "https://site.com".
  std::string to_origin(std::string url)
  {
    const size_t host = url.find("://");
    const size_t path = url.find('/', host == std::string::npos ? 0 : host + 3);
    if (path != std::string::npos) url.erase(path);
    return url;
  }
}  // namespace

ServerConfig& ServerConfig::get_instance()
{
  static ServerConfig instance;
  return instance;
}

ServerConfig::ServerConfig()
  : m_data_dir(get_default_data_dir()), m_port(DEFAULT_PORT),
    m_user(DEFAULT_USER), m_origins{DEFAULT_ORIGIN}
{
  const fs::path install_dir = get_install_dir();
  if (install_dir.empty())
  {
    log_error("Pasta de instalacao nao encontrada. Usando a configuracao padrao.");
    return;
  }
  m_file = install_dir / "config.json";

  std::error_code ec;
  const bool exists = fs::exists(m_file, ec) && fs::file_size(m_file, ec) > 0;
  if (exists && load()) return;

  if (exists) log_error("Configuracao incompleta ou invalida em " + quoted(m_file) + ". Regravada com as opcoes validas.");
  if (save()) log_info("Configuracao gravada em " + quoted(m_file) + ".");
}

// Le o arquivo. Cada opcao valida substitui o padrao. Retorna false se alguma
// faltar ou for invalida (o arquivo precisa ser regravado).
bool ServerConfig::load()
{
  std::vector<char> bytes;
  if (!read_bytes(m_file, bytes)) return true;   // nao conseguiu ler: nao sobrescreve

  std::string_view text(bytes.data(), bytes.size());
  if (text.substr(0, 3) == "\xEF\xBB\xBF") text.remove_prefix(3);   // BOM do Bloco de Notas

  json::Document doc;
  doc.Parse(text.data(), text.size());
  if (doc.HasParseError() || !doc.IsObject())
  {
    fs::path backup = m_file;
    backup += ".bak";
    std::error_code ec;
    fs::copy_file(m_file, backup, fs::copy_options::overwrite_existing, ec);
    log_error("Configuracao com JSON invalido (nos caminhos, use \\\\ ou /). Copia salva em " + quoted(backup) + ".");
    return false;
  }

  bool valid = true;

  auto it = doc.FindMember(KEY_DATA_DIR);
  if (it != doc.MemberEnd() && it->value.IsString() && it->value.GetStringLength() > 0)
    m_data_dir = fs::u8path(it->value.GetString());
  else
    valid = false;

  it = doc.FindMember(KEY_PORT);
  if (it != doc.MemberEnd() && it->value.IsInt() && is_valid_port(it->value.GetInt()))
    m_port = it->value.GetInt();
  else
    valid = false;

  it = doc.FindMember(KEY_USER);
  if (it != doc.MemberEnd() && it->value.IsString() && it->value.GetStringLength() > 0)
    m_user = it->value.GetString();
  else
    valid = false;

  it = doc.FindMember(KEY_ORIGINS);
  if (it != doc.MemberEnd() && it->value.IsArray())
  {
    m_origins.clear();
    for (const json::Value& v : it->value.GetArray())
    {
      if (!v.IsString() || v.GetStringLength() == 0) { valid = false; continue; }
      m_origins.push_back(to_origin(v.GetString()));
      if (m_origins.back() != v.GetString()) valid = false;   // tinha caminho: grava so a origem
    }
  }
  else
    valid = false;

  return valid;
}

bool ServerConfig::save() const
{
  if (m_file.empty()) return false;

  json::StringBuffer buf;
  json::PrettyWriter<json::StringBuffer> writer(buf);
  writer.StartObject();
  writer.Key(KEY_DATA_DIR);
  writer.String(m_data_dir.u8string());
  writer.Key(KEY_PORT);
  writer.Int(m_port);
  writer.Key(KEY_USER);
  writer.String(m_user);
  writer.Key(KEY_ORIGINS);
  writer.StartArray();
  for (const std::string& origin : m_origins) writer.String(origin);
  writer.EndArray();
  writer.EndObject();

  return write_byte(m_file, std::string_view(buf.GetString(), buf.GetSize()));
}

ServerConfig::Error ServerConfig::update(const fs::path& data_dir, int port, const std::string& user)
{
  std::error_code ec;
  if (data_dir.empty() || !fs::is_directory(data_dir, ec)) return Error::InvalidDataDir;
  if (!is_valid_port(port)) return Error::InvalidPort;
  if (user.empty()) return Error::InvalidUser;

  std::lock_guard<std::mutex> lock(m_mutex);

  const fs::path old_data_dir = m_data_dir;
  const int old_port = m_port;
  const std::string old_user = m_user;

  m_data_dir = data_dir;
  m_port = port;
  m_user = user;
  if (save())
  {
    log_info("Configuracao alterada: dados=" + quoted(m_data_dir) + ", porta=" + std::to_string(m_port) + ", usuario=" + m_user + ".");
    return Error::None;
  }

  m_data_dir = old_data_dir;   // arquivo nao gravado: memoria volta ao que era
  m_port = old_port;
  m_user = old_user;
  return Error::SaveFailed;
}

const char* ServerConfig::error_message(Error e)
{
  switch (e)
  {
    case Error::None:           return "Configuracao salva.";
    case Error::InvalidDataDir: return "A pasta de dados precisa ser uma pasta existente.";
    case Error::InvalidPort:    return "A porta precisa ser um numero entre 1 e 65535.";
    case Error::InvalidUser:    return "O usuario nao pode ser vazio.";
    case Error::SaveFailed:     return "Nao foi possivel gravar o arquivo de configuracao.";
  }
  return "Erro desconhecido.";
}
