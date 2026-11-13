cmake_minimum_required(VERSION 3.20)
project(nat_comp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# cpp-httplib é um único header (httplib.h). Baixamos só ele, sem puxar o
# CMake da lib (que tenta achar OpenSSL/zlib/etc. no sistema).
# Depois que compilar, troque "master" por uma tag/commit para fixar a versão.
include(FetchContent)
FetchContent_Declare(httplib_h
  URL https://raw.githubusercontent.com/yhirose/cpp-httplib/v0.65.0/httplib.h
  DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_MakeAvailable(httplib_h)

add_executable(nat_comp main.cpp server_app.cpp setup.cpp utils.cpp)
target_include_directories(nat_comp PRIVATE ${httplib_h_SOURCE_DIR})

if(WIN32)
  target_link_libraries(nat_comp PRIVATE ws2_32)

  set(ICON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../public/favicon.ico)
  configure_file(app.rc.in ${CMAKE_CURRENT_BINARY_DIR}/app.rc @ONLY)
  target_sources(nat_comp PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/app.rc)
  set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/app.rc PROPERTIES
    OBJECT_DEPENDS ${ICON_PATH})
endif()