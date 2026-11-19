#include "platform/sys_tray.h"
#include "utils.h"

#include <fstream>
#include <iostream>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <shellapi.h>

namespace
{
  constexpr UINT WM_TRAY = WM_APP + 1;
  constexpr const wchar_t* CLASS_NAME = L"SecretariaVirtualTray";

  enum MenuId : UINT { ID_RESTART = 1, ID_EXIT, ID_CONFIG };

  struct TrayState
  {
    fs::path config_file;
    NOTIFYICONDATAW icon{};
    UINT taskbar_created = 0;   // o Explorer reiniciou: o icone precisa voltar
    SysTray::Action action = SysTray::Action::Exit;
  };

  void open_config(const fs::path& file)
  {
    if (!fs::exists(file) && ensure_parent_dir(file)) std::ofstream{file};   // cria vazio

    const std::wstring args = L"\"" + file.wstring() + L"\"";
    ShellExecuteW(nullptr, L"open", L"notepad.exe", args.c_str(), nullptr, SW_SHOWNORMAL);
  }

  void show_menu(HWND hwnd)
  {
    HMENU menu = CreatePopupMenu();
    AppendMenuW(menu, MF_STRING, ID_RESTART, L"Reiniciar");
    AppendMenuW(menu, MF_STRING, ID_EXIT, L"Encerrar");
    AppendMenuW(menu, MF_STRING, ID_CONFIG, L"Abrir Configuração...");

    POINT pt;
    GetCursorPos(&pt);
    SetForegroundWindow(hwnd);   // sem isso o menu nao fecha ao clicar fora
    TrackPopupMenu(menu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, nullptr);
    DestroyMenu(menu);
  }

  void quit(HWND hwnd, TrayState& state, SysTray::Action action)
  {
    state.action = action;
    DestroyWindow(hwnd);
  }

  LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
  {
    auto* state = reinterpret_cast<TrayState*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (!state) return DefWindowProcW(hwnd, msg, wp, lp);

    if (msg == state->taskbar_created)
    {
      Shell_NotifyIconW(NIM_ADD, &state->icon);
      return 0;
    }

    switch (msg)
    {
      case WM_TRAY:
        if (lp == WM_RBUTTONUP || lp == WM_LBUTTONUP) show_menu(hwnd);
        return 0;

      case WM_COMMAND:
        switch (LOWORD(wp))
        {
          case ID_RESTART: quit(hwnd, *state, SysTray::Action::Restart); return 0;
          case ID_EXIT:    quit(hwnd, *state, SysTray::Action::Exit);    return 0;
          case ID_CONFIG:  open_config(state->config_file);              return 0;
        }
        break;

      case WM_DESTROY:
        Shell_NotifyIconW(NIM_DELETE, &state->icon);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
  }
}  // namespace

SysTray::Action SysTray::run(const fs::path& config_file)
{
  TrayState state;
  state.config_file = config_file;
  state.taskbar_created = RegisterWindowMessageW(L"TaskbarCreated");

  const HINSTANCE inst = GetModuleHandleW(nullptr);

  WNDCLASSW wc{};
  wc.lpfnWndProc = window_proc;
  wc.hInstance = inst;
  wc.lpszClassName = CLASS_NAME;
  RegisterClassW(&wc);

  // Janela invisivel, so para receber as mensagens do icone e do menu.
  HWND hwnd = CreateWindowW(CLASS_NAME, L"", WS_OVERLAPPED, 0, 0, 0, 0, nullptr, nullptr, inst, nullptr);
  if (!hwnd)
  {
    std::cout << "Nao foi possivel criar o icone da bandeja." << std::endl;
    return Action::Exit;
  }
  SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&state));

  // Icone do executavel (IDI_ICON1, de app.rc), no tamanho pequeno do sistema.
  NOTIFYICONDATAW& icon = state.icon;
  icon.cbSize = sizeof(icon);
  icon.hWnd = hwnd;
  icon.uID = 1;
  icon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  icon.uCallbackMessage = WM_TRAY;
  icon.hIcon = static_cast<HICON>(LoadImageW(inst, L"IDI_ICON1", IMAGE_ICON,
    GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0));
  lstrcpynW(icon.szTip, L"Secretaria Virtual", ARRAYSIZE(icon.szTip));

  if (!Shell_NotifyIconW(NIM_ADD, &icon))
  {
    std::cout << "Nao foi possivel criar o icone da bandeja." << std::endl;
    DestroyWindow(hwnd);
    return Action::Exit;
  }

  MSG msg;
  while (GetMessageW(&msg, nullptr, 0, 0) > 0)
  {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

  if (icon.hIcon) DestroyIcon(icon.hIcon);
  UnregisterClassW(CLASS_NAME, inst);
  return state.action;
}
