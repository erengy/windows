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
#include <shobjidl.h>

extern const DWORD WM_TASKBARCALLBACK;
extern const DWORD WM_TASKBARCREATED;
extern const DWORD WM_TASKBARBUTTONCREATED;

namespace win {

class Taskbar {
public:
  Taskbar();
  ~Taskbar();

  BOOL Create(HWND hwnd, UINT uid, HICON icon, LPCWSTR tooltip);
  BOOL Destroy();
  BOOL Modify(LPCWSTR tip);
  BOOL Tip(LPCWSTR text, LPCWSTR title, int icon_index);

private:
  HWND hwnd_;
  NOTIFYICONDATA data_;
};

////////////////////////////////////////////////////////////////////////////////

class TaskbarList {
public:
  TaskbarList();
  ~TaskbarList();

  void Initialize(HWND hwnd);
  void Release();
  void SetProgressState(TBPFLAG flag);
  void SetProgressValue(ULONGLONG value, ULONGLONG total);

private:
  HWND hwnd_;
  ITaskbarList3* taskbar_list_;
};

}  // namespace win
