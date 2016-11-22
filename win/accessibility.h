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

#include <string>
#include <vector>

#include <windows.h>
#include <OleAcc.h>

namespace win {

struct AccessibleChild {
  AccessibleChild();

  std::wstring name;
  DWORD role;
  std::wstring value;
  std::vector<AccessibleChild> children;
};

class AccessibleObject {
public:
  AccessibleObject();
  virtual ~AccessibleObject();

  HRESULT FromWindow(HWND hwnd, DWORD object_id = OBJID_CLIENT);
  void Release();

  HRESULT GetName(std::wstring& name, long child_id = CHILDID_SELF,
                  IAccessible* acc = nullptr);
  HRESULT GetRole(DWORD& role, long child_id = CHILDID_SELF,
                  IAccessible* acc = nullptr);
  HRESULT GetRole(std::wstring& role, long child_id = CHILDID_SELF,
                  IAccessible* acc = nullptr);
  HRESULT GetValue(std::wstring& value, long child_id = CHILDID_SELF,
                   IAccessible* acc = nullptr);

  HRESULT BuildChildren(std::vector<AccessibleChild>& children,
                        IAccessible* acc = nullptr, LPARAM param = 0L);
  HRESULT GetChildCount(long* child_count, IAccessible* acc = nullptr);

  virtual bool AllowChildTraverse(AccessibleChild& child, LPARAM param = 0L);

#ifdef _DEBUG
  HWINEVENTHOOK Hook(DWORD eventMin, DWORD eventMax,
                     HMODULE hmodWinEventProc, WINEVENTPROC pfnWinEventProc,
                     DWORD idProcess, DWORD idThread, DWORD dwFlags);
  bool IsHooked();
  void Unhook();
#endif

  std::vector<AccessibleChild> children;

private:
  IAccessible* acc_;
  HWINEVENTHOOK win_event_hook_;
};

}  // namespace win
