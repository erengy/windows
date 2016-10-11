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

#pragma once

#include <windows.h>
#include <commctrl.h>
#include <uxtheme.h>

#include <map>
#include <string>
#include <vector>

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

namespace win {

class Window;

class WindowMap {
public:
  void Add(HWND hwnd, Window* window);
  void Clear();
  Window* GetWindow(HWND hwnd);
  void Remove(HWND hwnd);
  void Remove(Window* window);

private:
  std::map<HWND, Window*> window_map_;
};

extern WindowMap window_map;

////////////////////////////////////////////////////////////////////////////////

class App {
public:
  App();
  virtual ~App();

  BOOL InitCommonControls(DWORD flags) const;
  virtual BOOL InitInstance();
  virtual int MessageLoop();
  virtual void PostQuitMessage(int exit_code = 0);
  virtual int Run();

  std::wstring GetCurrentDirectory() const;
  HINSTANCE GetInstanceHandle() const;
  std::wstring GetModulePath() const;
  BOOL SetCurrentDirectory(const std::wstring& directory);

private:
  HINSTANCE instance_;
};

////////////////////////////////////////////////////////////////////////////////

enum Version {
  kVersionPreXp = 0,
  kVersionXp,
  kVersionServer2003,
  kVersionVista,
  kVersionServer2008,
  kVersion7,
  kVersion8,
  kVersion8_1,
  kVersion10,
  kVersionUnknown
};

Version GetVersion();

}  // namespace win
