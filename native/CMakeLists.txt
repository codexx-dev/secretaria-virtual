cmake_minimum_required(VERSION 3.20)
project(secretaria_servidor LANGUAGES CXX)

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

add_executable(servidor main.cpp)
target_include_directories(servidor PRIVATE ${httplib_h_SOURCE_DIR})

if(WIN32)
  target_link_libraries(servidor PRIVATE ws2_32)
endif()