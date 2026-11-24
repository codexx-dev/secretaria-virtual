#pragma once

#include <filesystem>
#include <string>
#include <system_error>

namespace fs = std::filesystem;

// Log do app em <instalacao>/log.txt, uma linha por evento (UTF-8):
//   2026-11-24 14:05:03 [INFO] Servidor em http://127.0.0.1:8080
//   2026-11-24 14:05:09 [ERRO] Falha ao ler "C:/.../config.json": acesso negado
//
// Compilado com LOG_TO_CONSOLE, tambem escreve no console.
void log_info(const std::string& message);
void log_error(const std::string& message);

// Texto para as mensagens: caminho entre aspas e descricao de um erro do sistema.
std::string quoted(const fs::path& path);
std::string describe(const std::error_code& ec);
