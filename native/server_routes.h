#pragma once

#include <httplib.h>

// Registra todas as rotas da API no servidor.
void register_routes(httplib::Server& svr);
