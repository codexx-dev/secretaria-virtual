#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

// ----------------------------------------------------------------------------
// Arquivos simples (sem trava)
// ----------------------------------------------------------------------------

// Cria a pasta do arquivo (e as pastas acima dela) se nao existir.
bool ensure_parent_dir(const fs::path& path);

// Grava o arquivo inteiro: escreve em "<arquivo>.tmp" e troca pelo original.
bool write_byte(const fs::path& path, std::string_view data);

bool erase(const fs::path& path);

bool read_bytes(const fs::path& path, std::vector<char>& out);

// ----------------------------------------------------------------------------
// Caminhos dentro da pasta de dados (ServerConfig)
// ----------------------------------------------------------------------------

// true se "path" esta DENTRO da pasta de dados (a propria pasta nao conta).
// Resolve "..", "." e links simbolicos antes de comparar, e ignora diferenca
// de maiusculas/minusculas conforme o sistema de arquivos. Falso se a pasta
// de dados nao existir ou nao puder ser descoberta.
bool is_inside_data_dir(const fs::path& path);
