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

set(NAT_COMP_SOURCES main.cpp server_app.cpp setup.cpp utils.cpp locked_file.cpp csv_file.cpp db.cpp)

# Ícone do executável (compartilhado pelos dois alvos).
set(ICON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../public/favicon.ico)
configure_file(app.rc.in ${CMAKE_CURRENT_BINARY_DIR}/app.rc @ONLY)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/app.rc PROPERTIES
  OBJECT_DEPENDS ${ICON_PATH})

# Cria um alvo do componente nativo.
#   target      : nome do alvo/executável
#   httplib_dir : pasta com o httplib.h a usar
#   winnt       : versão mínima do Windows (_WIN32_WINNT)
function(add_nat_comp target httplib_dir winnt)
  add_executable(${target} ${NAT_COMP_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/app.rc)
  target_include_directories(${target} PRIVATE ${httplib_dir} ${rapidjson_SOURCE_DIR}/include)
  # Restringe os headers do Windows às APIs disponíveis na versão alvo.
  # RAPIDJSON_HAS_STDSTRING: permite usar std::string direto com o rapidjson.
  target_compile_definitions(${target} PRIVATE
    _WIN32_WINNT=${winnt}
    WINVER=${winnt}
    RAPIDJSON_HAS_STDSTRING=1
  )
  target_link_libraries(${target} PRIVATE ws2_32)

  # Linkagem estática: embute a runtime do C++ (libc++/libstdc++, libunwind,
  # winpthread) no exe, para não depender de DLLs que não vêm com o Windows.
  if(MSVC)
    set_property(TARGET ${target} PROPERTY
      MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  else()
    target_link_options(${target} PRIVATE -static)
  endif()
endfunction()

# 1) Windows 8   (_WIN32_WINNT_WIN8  = 0x0602)
add_nat_comp(nat_comp_win8  ${httplib_win8_SOURCE_DIR}  0x0602)

# 2) Windows 10 e 11 (_WIN32_WINNT_WIN10 = 0x0A00)
add_nat_comp(nat_comp_win10 ${httplib_win10_SOURCE_DIR} 0x0A00)
