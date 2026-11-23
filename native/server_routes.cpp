#include "server_routes.h"
#include "server_config.h"
#include "db.h"

#include <cstdlib>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace json = rapidjson;

namespace
{
  // Resposta de erro: {"err": "<mensagem>"}.
  void send_error(httplib::Response& res, int status, const std::string& message)
  {
    json::StringBuffer buf;
    json::Writer<json::StringBuffer> writer(buf);
    writer.StartObject();
    writer.Key("err");
    writer.String(message);
    writer.EndObject();

    res.status = status;
    res.set_content(buf.GetString(), buf.GetSize(), "application/json");
  }

  void send_db_error(httplib::Response& res, DbStatus st)
  {
    int status = 500;
    switch (st)
    {
      case DbStatus::InvalidName:
      case DbStatus::InvalidColumns:
      case DbStatus::UnknownColumn:  status = 400; break;
      case DbStatus::NotFound:
      case DbStatus::RowNotFound:    status = 404; break;
      case DbStatus::AlreadyExists:
      case DbStatus::DuplicateRow:   status = 409; break;
      case DbStatus::Locked:         status = 423; break;
      default:                       break;
    }
    send_error(res, status, db_status_message(st));
  }

  // "PASTA/NOME" (UTF-8) -> <pasta de dados>/PASTA/NOME.csv
  fs::path table_path(const std::string& src)
  {
    fs::path path = ServerConfig::get_instance().data_dir() / fs::u8path(src);
    path += ".csv";
    return path;
  }

  // Le o corpo JSON (um objeto). Em erro, ja responde 400.
  bool parse_json(const httplib::Request& req, httplib::Response& res, json::Document& body)
  {
    body.Parse<json::kParseValidateEncodingFlag>(req.body.c_str(), req.body.size());   // recusa texto que nao e UTF-8
    if (body.HasParseError() || !body.IsObject())
    {
      send_error(res, 400, "O corpo da requisicao nao e um JSON valido.");
      return false;
    }
    return true;
  }

  // Le o corpo JSON e o caminho da tabela ("src"). Em erro, ja responde 400.
  bool parse_body(const httplib::Request& req, httplib::Response& res,
                  json::Document& body, fs::path& path)
  {
    if (!parse_json(req, res, body)) return false;

    if (!body.HasMember("src") || !body["src"].IsString() || body["src"].GetStringLength() == 0)
    {
      send_error(res, 400, "Informe \"src\".");
      return false;
    }

    path = table_path(body["src"].GetString());
    return true;
  }

  // body[key] = {"coluna": "valor", ...} -> Row. Em erro, ja responde 400.
  bool parse_row(const json::Document& body, const char* key, httplib::Response& res, Row& row)
  {
    const auto it = body.FindMember(key);
    bool ok = it != body.MemberEnd() && it->value.IsObject();

    if (ok)
      for (auto f = it->value.MemberBegin(); f != it->value.MemberEnd(); ++f)   // GetObject() e macro do windows.h
      {
        if (!f->value.IsString()) { ok = false; break; }
        row.emplace_back(f->name.GetString(), f->value.GetString());
      }

    if (!ok) send_error(res, 400, std::string("\"") + key + "\" deve ser um objeto com valores de texto.");
    return ok;
  }

  // --------------------------------------------------------------------------
  // Teste
  // --------------------------------------------------------------------------

  void ping(const httplib::Request&, httplib::Response& res)
  {
    res.set_content("pong", "text/plain");
  }

  // --------------------------------------------------------------------------
  // Dados
  // --------------------------------------------------------------------------

  // GET /api/dados?src=PASTA/NOME
  void get_data(const httplib::Request& req, httplib::Response& res)
  {
    const std::string src = req.get_param_value("src");
    if (src.empty())
    {
      send_error(res, 400, "Informe \"src\".");
      return;
    }

    CsvTable table;
    const DbStatus st = DB::read(table_path(src), table);
    if (st != DbStatus::Ok) return send_db_error(res, st);

    res.set_content(csv_to_json(table), "application/json");
  }

  // POST /api/dados  {"src": "...", "new": {...}}
  void create_data(const httplib::Request& req, httplib::Response& res)
  {
    json::Document body;
    fs::path path;
    Row new_row;
    if (!parse_body(req, res, body, path) || !parse_row(body, "new", res, new_row)) return;

    const DbStatus st = DB::insert_val(path, new_row);
    if (st != DbStatus::Ok) return send_db_error(res, st);

    res.status = 201;
  }

  // PUT /api/dados  {"src": "...", "old": {...}, "new": {...}}
  void update_data(const httplib::Request& req, httplib::Response& res)
  {
    json::Document body;
    fs::path path;
    Row old_row, new_row;
    if (!parse_body(req, res, body, path) ||
        !parse_row(body, "old", res, old_row) ||
        !parse_row(body, "new", res, new_row)) return;

    const DbStatus st = DB::update_val(path, old_row, new_row);
    if (st != DbStatus::Ok) return send_db_error(res, st);

    res.status = 204;
  }

  // DELETE /api/dados  {"src": "...", "old": {...}}      apaga a linha
  //                    {"src": "...", "delete-all": true}  apaga a tabela inteira
  void delete_data(const httplib::Request& req, httplib::Response& res)
  {
    json::Document body;
    fs::path path;
    if (!parse_body(req, res, body, path)) return;

    const auto all = body.FindMember("delete-all");
    const bool delete_all = all != body.MemberEnd() && all->value.IsBool() && all->value.GetBool();

    DbStatus st;
    if (delete_all)
      st = DB::remove(path);
    else
    {
      Row old_row;
      if (!parse_row(body, "old", res, old_row)) return;
      st = DB::delete_val(path, old_row);
    }
    if (st != DbStatus::Ok) return send_db_error(res, st);

    res.status = 204;
  }

  // --------------------------------------------------------------------------
  // Configuracao
  // --------------------------------------------------------------------------

  // {"data": "...", "port": 8080, "user": "..."}
  void send_config(httplib::Response& res)
  {
    const ServerConfig& config = ServerConfig::get_instance();

    json::StringBuffer buf;
    json::Writer<json::StringBuffer> writer(buf);
    writer.StartObject();
    writer.Key("data");
    writer.String(config.data_dir().u8string());
    writer.Key("port");
    writer.Int(config.port());
    writer.Key("user");
    writer.String(config.user());
    writer.EndObject();

    res.set_content(buf.GetString(), buf.GetSize(), "application/json");
  }

  // GET /api/configuracao
  void get_config(const httplib::Request&, httplib::Response& res)
  {
    send_config(res);
  }

  // POST /api/configuracao  {"data": "...", "port": 8080, "user": "..."}
  // Cada chave e opcional: a que faltar mantem o valor atual. A validacao e a
  // gravacao ficam na ServerConfig; aqui so se le o JSON.
  void post_config(const httplib::Request& req, httplib::Response& res)
  {
    json::Document body;
    if (!parse_json(req, res, body)) return;

    ServerConfig& config = ServerConfig::get_instance();
    fs::path data_dir = config.data_dir();
    int port = config.port();
    std::string user = config.user();

    // Tipo errado vira valor vazio/zero, que a ServerConfig recusa.
    if (const auto it = body.FindMember("data"); it != body.MemberEnd())
      data_dir = it->value.IsString() ? fs::u8path(it->value.GetString()) : fs::path();

    if (const auto it = body.FindMember("port"); it != body.MemberEnd())
      port = it->value.IsInt()    ? it->value.GetInt()
           : it->value.IsString() ? std::atoi(it->value.GetString())   // aceita "8080"
           : 0;

    if (const auto it = body.FindMember("user"); it != body.MemberEnd())
      user = it->value.IsString() ? it->value.GetString() : "";

    const ServerConfig::Error err = config.update(data_dir, port, user);
    if (err == ServerConfig::Error::SaveFailed) return send_error(res, 500, ServerConfig::error_message(err));
    if (err != ServerConfig::Error::None)       return send_error(res, 400, ServerConfig::error_message(err));

    send_config(res);
  }
}  // namespace

void register_routes(httplib::Server& svr)
{
  svr.Get("/api/ping", ping);

  svr.Get("/api/dados", get_data);
  svr.Post("/api/dados", create_data);
  svr.Put ("/api/dados", update_data);
  svr.Delete("/api/dados", delete_data);

  svr.Get("/api/configuracao", get_config);
  svr.Post("/api/configuracao", post_config);
}
