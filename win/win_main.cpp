/*
MIT License

Copyright (c) 2010-2016 Eren Okka

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "win_main.h"
#include "win_window.h"
#include "win_windowmap.h"

namespace win {

App::App() {
  instance_ = ::GetModuleHandle(nullptr);
}

App::~App() {
  window_map.Clear();
}

BOOL App::InitCommonControls(DWORD flags) const {
  INITCOMMONCONTROLSEX icc;
  icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icc.dwICC = flags;

  return ::InitCommonControlsEx(&icc);
}

BOOL App::InitInstance() {
  return TRUE;
}

int App::MessageLoop() {
  MSG msg;

  while (::GetMessage(&msg, nullptr, 0, 0)) {
    BOOL processed = FALSE;
    if ((msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST) ||
        (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)) {
      for (HWND hwnd = msg.hwnd; hwnd != nullptr; hwnd = ::GetParent(hwnd)) {
        auto window = window_map.GetWindow(hwnd);
        if (window) {
          processed = window->PreTranslateMessage(&msg);
          if (processed)
            break;
        }
      }
    }

    if (!processed) {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }
  }

  return static_cast<int>(LOWORD(msg.wParam));
}

void App::PostQuitMessage(int exit_code) {
  ::PostQuitMessage(exit_code);
}

int App::Run() {
  if (InitInstance()) {
    return MessageLoop();
  } else {
    ::PostQuitMessage(-1);
    return -1;
  }
}

std::wstring App::GetCurrentDirectory() const {
  WCHAR path[MAX_PATH];
  DWORD len = ::GetCurrentDirectory(MAX_PATH, path);
  if (len > 0 && len < MAX_PATH)
    return path;

  return std::wstring();
}

HINSTANCE App::GetInstanceHandle() const {
  return instance_;
}

std::wstring App::GetModulePath() const {
  WCHAR path[MAX_PATH];
  ::GetModuleFileName(instance_, path, MAX_PATH);
  return path;
}

BOOL App::SetCurrentDirectory(const std::wstring& directory) {
  return ::SetCurrentDirectory(directory.c_str());
}

////////////////////////////////////////////////////////////////////////////////

Version GetVersion() {
  static bool checked = false;
  static Version version = kVersionPreXp;

  if (!checked) {
    OSVERSIONINFOEX version_info;
    version_info.dwOSVersionInfoSize = sizeof(version_info);
    GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&version_info));

    if (version_info.dwMajorVersion == 5) {
      switch (version_info.dwMinorVersion) {
        case 0:
          version = kVersionPreXp;
          break;
        case 1:
          version = kVersionXp;
          break;
        case 2:
        default:
          version = kVersionServer2003;
          break;
      }
    } else if (version_info.dwMajorVersion == 6) {
      if (version_info.wProductType != VER_NT_WORKSTATION) {
        version = kVersionServer2008;
      } else {
        switch (version_info.dwMinorVersion) {
          case 0:
            version = kVersionVista;
            break;
          case 1:
            version = kVersion7;
            break;
          case 2:
            version = kVersion8;
            break;
          case 3:
          default:
            version = kVersion8_1;
            break;
        }
      }
    } else if (version_info.dwMajorVersion == 10) {
      switch (version_info.dwMinorVersion) {
        case 0:
        default:
          version = kVersion10;
          break;
      }
    } else if (version_info.dwMajorVersion > 10) {
      version = kVersionUnknown;
    }

    checked = true;
  }

  return version;
}

}  // namespace win
