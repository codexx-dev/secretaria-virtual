cmake_minimum_required(VERSION 3.20)
project(nat_comp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------------------------------------------------------------
# cpp-httplib: é um único header (httplib.h). Baixamos só ele, sem puxar o
# CMake da lib (que tenta achar OpenSSL/zlib/etc. no sistema).
#
# Duas versões, uma por alvo:
#   - v0.22.0: última sem nenhuma restrição de versão do Windows. A v0.23.0
#     removeu o suporte a Windows 8 e a v0.25.0 passou a exigir Windows 10.
#   - v0.65.0: versão atual, para Windows 10 e 11.
# ----------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(httplib_win8
  URL https://raw.githubusercontent.com/yhirose/cpp-httplib/v0.22.0/httplib.h
  DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_Declare(httplib_win10
  URL https://raw.githubusercontent.com/yhirose/cpp-httplib/v0.65.0/httplib.h
  DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_MakeAvailable(httplib_win8 httplib_win10)

# ----------------------------------------------------------------------------
# rapidjson: só headers. A última release (v1.1.0, 2016) não compila nos
# compiladores atuais, então fixamos um commit do master.
# SOURCE_SUBDIR aponta para uma pasta inexistente para o CMake só baixar,
# sem rodar o CMakeLists da lib (antigo, quebra no CMake 4 e compila testes).
# ----------------------------------------------------------------------------
FetchContent_Declare(rapidjson
  URL https://github.com/Tencent/rapidjson/archive/297637bc0a0f00dc504dbc1a47ebdaa577f8ae19.tar.gz
  SOURCE_SUBDIR _none
)
FetchContent_MakeAvailable(rapidjson)

# Codigo portavel (raiz de native/). Caminhos absolutos: a plataforma monta os
# alvos em outro diretorio.
set(NAT_COMP_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/app_config.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/server_app.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/setup.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/csv_file.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/db.cpp
)

# Definicoes comuns a todas as plataformas.
set(NAT_COMP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${rapidjson_SOURCE_DIR}/include)
set(NAT_COMP_DEFINITIONS RAPIDJSON_HAS_STDSTRING=1)   # std::string direto no rapidjson

# Icone do executavel (public/ do frontend).
set(ICON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../public/favicon.ico)

# ----------------------------------------------------------------------------
# Plataforma: native/platform/<pasta>/ implementa o que esta em platform.h e
# locked_file.h, e define os alvos (cada plataforma sabe como monta os seus).
# ----------------------------------------------------------------------------
if(WIN32)
  set(NAT_COMP_PLATFORM win)
else()
  message(FATAL_ERROR "Plataforma sem suporte: ${CMAKE_SYSTEM_NAME}")
endif()

# Executaveis na raiz do build, independente da pasta da plataforma.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(platform/${NAT_COMP_PLATFORM})
