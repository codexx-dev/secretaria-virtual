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

  constexpr int DEFAULT_PORT = 8080;
  constexpr const char* DEFAULT_USER = "default";

  constexpr const char* APP_DIR_NAME = "SecretariaVirtual";

  bool is_valid_port(int port) { return port > 0 && port <= 65535; }

  // Os dados ficam numa pasta do app dentro da pasta escolhida pelo usuario:
  // "Documentos" -> "Documentos/SecretariaVirtual". Se ja for ela, fica igual.
  fs::path app_data_dir(fs::path chosen)
  {
    if (!chosen.has_filename()) chosen = chosen.parent_path();   // "Documentos/"
    return chosen.filename() == APP_DIR_NAME ? chosen : chosen / APP_DIR_NAME;
  }

  // Pasta que o usuario pode escolher: existe e fica na pasta do usuario (ou e ela) ou na rede.
  bool is_allowed_location(const fs::path& dir)
  {
    std::error_code ec;
    if (dir.empty() || !fs::is_directory(dir, ec)) return false;
    return is_within(dir, get_user_dir()) || is_network_path(dir);
  }
}  // namespace

ServerConfig& ServerConfig::get_instance()
{
  static ServerConfig instance;
  return instance;
}

ServerConfig::ServerConfig()
  : m_data_dir(get_default_data_dir()), m_port(DEFAULT_PORT),
    m_user(DEFAULT_USER)
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

  // Chave a mais (ex.: "origins", que saiu do arquivo): regrava sem ela.
  if (doc.MemberCount() != 3) valid = false;

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
  writer.EndObject();

  return write_byte(m_file, std::string_view(buf.GetString(), buf.GetSize()));
}

ServerConfig::Error ServerConfig::update(const fs::path& chosen_dir, int port, const std::string& user)
{
  if (!is_valid_port(port)) return Error::InvalidPort;
  if (user.empty()) return Error::InvalidUser;

  // A pasta escolhida e validada; os dados vao para a pasta do app dentro dela.
  const fs::path data_dir = app_data_dir(chosen_dir);
  if (!is_allowed_location(data_dir.parent_path())) return Error::InvalidDataDir;

  std::error_code ec;
  fs::create_directory(data_dir, ec);
  if (!fs::is_directory(data_dir, ec) || !can_read_write(data_dir)) return Error::InvalidDataDir;

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
    case Error::InvalidDataDir: return "A pasta escolhida precisa existir, ficar na pasta do usuario ou na rede, e permitir leitura e escrita.";
    case Error::InvalidPort:    return "A porta precisa ser um numero entre 1 e 65535.";
    case Error::InvalidUser:    return "O usuario nao pode ser vazio.";
    case Error::SaveFailed:     return "Nao foi possivel gravar o arquivo de configuracao.";
  }
  return "Erro desconhecido.";
}
