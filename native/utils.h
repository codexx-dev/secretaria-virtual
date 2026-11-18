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
// Conversao de texto
// ----------------------------------------------------------------------------

// UTF-8 -> UTF-16 (caminhos do Windows). Falha se o UTF-8 for invalido.
bool utf8_to_wide(std::string_view in, std::wstring& out);

// UTF-16 -> UTF-8.
bool wide_to_utf8(std::wstring_view in, std::string& out);

// Texto em uma pagina de codigo (ex.: 1252, o "ANSI" do Excel pt-BR) -> UTF-8.
bool codepage_to_utf8(std::string_view in, unsigned codepage, std::string& out);


//
//
//

fs::path get_install_dir();
fs::path get_data_dir();