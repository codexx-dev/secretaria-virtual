# Plataforma Windows: implementa platform.h e locked_file.h e define os alvos.
# Variaveis herdadas de native/CMakeLists.txt: NAT_COMP_SOURCES,
# NAT_COMP_INCLUDE_DIRS, NAT_COMP_DEFINITIONS, ICON_PATH e as pastas das libs.

set(NAT_COMP_PLATFORM_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/locked_file.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sys_tray.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/system.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/text.cpp
)

# Recurso do executavel (icone e versao), compartilhado pelos dois alvos.
configure_file(app.rc.in ${CMAKE_CURRENT_BINARY_DIR}/app.rc @ONLY)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/app.rc PROPERTIES
  OBJECT_DEPENDS ${ICON_PATH})

# Cria um alvo do componente nativo.
#   target      : nome do alvo/executavel
#   httplib_dir : pasta com o httplib.h a usar
#   winnt       : versao minima do Windows (_WIN32_WINNT)
function(add_nat_comp target httplib_dir winnt)
  add_executable(${target}
    ${NAT_COMP_SOURCES}
    ${NAT_COMP_PLATFORM_SOURCES}
    ${CMAKE_CURRENT_BINARY_DIR}/app.rc)
  target_include_directories(${target} PRIVATE ${httplib_dir} ${NAT_COMP_INCLUDE_DIRS})
  # Restringe os headers do Windows as APIs disponiveis na versao alvo.
  target_compile_definitions(${target} PRIVATE
    _WIN32_WINNT=${winnt}
    WINVER=${winnt}
    ${NAT_COMP_DEFINITIONS}
  )
  target_link_libraries(${target} PRIVATE ws2_32 shell32)

  # Sem log no console: aplicativo de janela, que abre sem a janela do console.
  if(NOT LOG_TO_CONSOLE)
    set_property(TARGET ${target} PROPERTY WIN32_EXECUTABLE TRUE)
    if(MSVC)
      target_link_options(${target} PRIVATE /ENTRY:mainCRTStartup)   # continua usando main()
    endif()
  endif()

  # Linkagem estatica: embute a runtime do C++ (libc++/libstdc++, libunwind,
  # winpthread) no exe, para nao depender de DLLs que nao vem com o Windows.
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
