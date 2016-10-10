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

Edit::Edit(HWND hwnd) {
  SetWindowHandle(hwnd);
}

void Edit::PreCreate(CREATESTRUCT &cs) {
  cs.dwExStyle = WS_EX_CLIENTEDGE;
  cs.lpszClass = L"EDIT";
  cs.style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL;
}

void Edit::OnCreate(HWND hwnd, LPCREATESTRUCT create_struct) {
  Window::OnCreate(hwnd, create_struct);
}

////////////////////////////////////////////////////////////////////////////////

void Edit::GetRect(LPRECT rect) {
  SendMessage(EM_GETRECT, 0, reinterpret_cast<LPARAM>(rect));
}

void Edit::LimitText(int limit) {
  SendMessage(EM_SETLIMITTEXT, limit, 0);
}

BOOL Edit::SetCueBannerText(LPCWSTR text, BOOL draw_focused) {
  return SendMessage(EM_SETCUEBANNER, draw_focused,
                     reinterpret_cast<LPARAM>(text));
}

void Edit::SetMargins(int left, int right) {
  DWORD flags = 0;

  if (left > -1)
    flags |= EC_LEFTMARGIN;
  if (right > -1)
    flags |= EC_RIGHTMARGIN;

  SendMessage(EM_SETMARGINS, flags, MAKELPARAM(left, right));
}

void Edit::SetMultiLine(BOOL enabled) {
  // TODO: We have to re-create the control, this does not work.
  if (enabled) {
    SetStyle(ES_MULTILINE | ES_AUTOVSCROLL, ES_AUTOHSCROLL);
  } else {
    SetStyle(ES_AUTOHSCROLL, ES_MULTILINE | ES_AUTOVSCROLL);
  }
}

void Edit::SetPasswordChar(UINT ch) {
  SendMessage(EM_SETPASSWORDCHAR, ch, 0);
}

void Edit::SetReadOnly(BOOL read_only) {
  SendMessage(EM_SETREADONLY, read_only, 0);
}

void Edit::SetRect(LPRECT rect) {
  SendMessage(EM_SETRECT, 0, reinterpret_cast<LPARAM>(rect));
}

void Edit::SetSel(int start, int end) {
  SendMessage(EM_SETSEL, start, end);
  SendMessage(EM_SCROLLCARET, 0, 0);
}

BOOL Edit::ShowBalloonTip(LPCWSTR text, LPCWSTR title, INT icon) {
  EDITBALLOONTIP ebt;
  ebt.cbStruct = sizeof(EDITBALLOONTIP);
  ebt.pszText = text;
  ebt.pszTitle = title;
  ebt.ttiIcon = icon;

  return SendMessage(EM_SHOWBALLOONTIP, 0, reinterpret_cast<LPARAM>(&ebt));
}

}  // namespace win