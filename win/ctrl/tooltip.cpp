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

#include "win_ctrl.h"

namespace win {

Tooltip::Tooltip(HWND hwnd) {
  SetWindowHandle(hwnd);
}

void Tooltip::PreCreate(CREATESTRUCT &cs) {
  cs.dwExStyle = 0;
  cs.lpszClass = TOOLTIPS_CLASS;
  cs.style = WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP;
}

void Tooltip::OnCreate(HWND hwnd, LPCREATESTRUCT create_struct) {
  ::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
  Window::OnCreate(hwnd, create_struct);
}

////////////////////////////////////////////////////////////////////////////////

BOOL Tooltip::AddTip(UINT id, LPCWSTR text, LPCWSTR title, LPRECT rect,
                     bool window_id) {
  TOOLINFO ti = {0};
  ti.cbSize = sizeof(TOOLINFO);
  ti.hwnd = parent_;
  ti.hinst = instance_;
  ti.lpszText = const_cast<LPWSTR>(text);
  ti.uFlags = TTF_SUBCLASS | (window_id ? TTF_IDISHWND : 0);
  ti.uId = static_cast<UINT_PTR>(id);

  if (rect)
    ti.rect = *rect;

  BOOL result = SendMessage(TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&ti));
  if (result && title)
    result = SendMessage(TTM_SETTITLE, TTI_INFO,
                         reinterpret_cast<LPARAM>(title));

  return result;
}

void Tooltip::DeleteTip(UINT id) {
  TOOLINFO ti = {0};
  ti.cbSize = sizeof(TOOLINFO);
  ti.hwnd = parent_;
  ti.uId = static_cast<UINT_PTR>(id);

  SendMessage(TTM_DELTOOL, 0, reinterpret_cast<LPARAM>(&ti));
}

void Tooltip::NewToolRect(UINT id, LPRECT rect) {
  TOOLINFO ti = {0};
  ti.cbSize = sizeof(TOOLINFO);
  ti.hwnd = parent_;
  ti.uId = static_cast<UINT_PTR>(id);

  if (rect)
    ti.rect = *rect;

  SendMessage(TTM_NEWTOOLRECT, 0, reinterpret_cast<LPARAM>(&ti));
}

void Tooltip::SetDelayTime(long autopop, long initial, long reshow) {
  SendMessage(TTM_SETDELAYTIME, TTDT_AUTOPOP, autopop);
  SendMessage(TTM_SETDELAYTIME, TTDT_INITIAL, initial);
  SendMessage(TTM_SETDELAYTIME, TTDT_RESHOW,  reshow);
}

void Tooltip::SetMaxWidth(long width) {
  SendMessage(TTM_SETMAXTIPWIDTH, 0, width);
}

void Tooltip::UpdateText(UINT id, LPCWSTR text) {
  TOOLINFO ti = {0};
  ti.cbSize = sizeof(TOOLINFO);
  ti.hinst = instance_;
  ti.hwnd = parent_;
  ti.lpszText = const_cast<LPWSTR>(text);
  ti.uId = static_cast<UINT_PTR>(id);

  SendMessage(TTM_UPDATETIPTEXT, 0, reinterpret_cast<LPARAM>(&ti));
}

void Tooltip::UpdateTitle(LPCWSTR title) {
  SendMessage(TTM_SETTITLE, title ? 0 : 1, reinterpret_cast<LPARAM>(title));
}

}  // namespace win