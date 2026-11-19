#pragma once

// ----------------------------------------------------------------------------
// Interface comum das plataformas.
//
// O codigo fora de native/platform/ so conhece estas declaracoes (e o
// locked_file.h). Cada plataforma fornece a implementacao em
// native/platform/<plataforma>/, escolhida pelo CMake.
// ----------------------------------------------------------------------------

#include <filesystem>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

// ----------------------------------------------------------------------------
// Conversao de texto
// ----------------------------------------------------------------------------

// UTF-8 -> UTF-16 (caminhos do Windows). Falha se o UTF-8 for invalido.
bool utf8_to_wide(std::string_view in, std::wstring& out);

// UTF-16 -> UTF-8.
bool wide_to_utf8(std::wstring_view in, std::string& out);

// Texto em uma pagina de codigo (ex.: 1252, o "ANSI" do Excel pt-BR) -> UTF-8.
bool codepage_to_utf8(std::string_view in, unsigned codepage, std::string& out);

// ----------------------------------------------------------------------------
// Pastas do app
// ----------------------------------------------------------------------------

// Pasta onde o executavel e instalado. Vazio se nao for possivel descobrir.
fs::path get_install_dir();

// Pasta padrao dos dados (CSV/Excel) do usuario. Vazio se nao for possivel descobrir.
fs::path get_default_data_dir();

// ----------------------------------------------------------------------------
// Executavel e inicializacao automatica
// ----------------------------------------------------------------------------

// Caminho do executavel em execucao. Falha (false) se nao for possivel descobrir.
bool get_executable_path(fs::path& out);

// Nome do arquivo executavel instalado.
const fs::path& executable_name();

// Registra o executavel para iniciar com o sistema do usuario atual.
// Em caso de falha, "error" recebe o codigo do sistema.
bool enable_autostart(const fs::path& exe, long& error);

// Remove o registro. Nao e erro se ele ja nao existir.
bool disable_autostart(long& error);

// Inicia uma nova instancia do executavel.
bool start_new_instance();
