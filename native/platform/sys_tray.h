#pragma once

#include <filesystem>

namespace fs = std::filesystem;

// Icone na bandeja do sistema. Cada plataforma implementa em
// native/platform/<plataforma>/sys_tray.cpp.
class SysTray
{
public:
  enum class Action { Restart, Exit };

  // Mostra o icone e atende o menu ate o usuario escolher Reiniciar ou
  // Encerrar. "Abrir Configuracao..." abre config_file no Bloco de Notas.
  static Action run(const fs::path& config_file);
};
