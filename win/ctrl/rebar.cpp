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

Rebar::Rebar(HWND hwnd) {
  SetWindowHandle(hwnd);
}

void Rebar::PreCreate(CREATESTRUCT &cs) {
  cs.dwExStyle = 0;
  cs.lpszClass = REBARCLASSNAME;
  cs.style = WS_CHILD | WS_VISIBLE | WS_TABSTOP |
             CCS_NODIVIDER | RBS_BANDBORDERS | RBS_VARHEIGHT;
}

void Rebar::OnCreate(HWND hwnd, LPCREATESTRUCT create_struct) {
  Window::OnCreate(hwnd, create_struct);
}

////////////////////////////////////////////////////////////////////////////////

UINT Rebar::GetBarHeight() {
  return SendMessage(RB_GETBARHEIGHT, 0, 0);
}

BOOL Rebar::InsertBand(LPREBARBANDINFO bar_info) {
  return SendMessage(RB_INSERTBAND, -1, reinterpret_cast<LPARAM>(bar_info));
}

BOOL Rebar::InsertBand(HWND hwnd_child,
                       UINT cx, UINT cx_header, UINT cx_ideal,
                       UINT cx_min_child,
                       UINT cy_child, UINT cy_integral,
                       UINT cy_max_child, UINT cy_min_child,
                       UINT mask, UINT style) {
  REBARBANDINFO rbi = {0};
  rbi.cbSize = REBARBANDINFO_V6_SIZE;
  rbi.cx = cx;
  rbi.cxHeader = cx_header;
  rbi.cxIdeal = cx_ideal;
  rbi.cxMinChild = cx_min_child;
  rbi.cyChild = cy_child;
  rbi.cyIntegral = cy_integral;
  rbi.cyMaxChild = cy_max_child;
  rbi.cyMinChild = cy_min_child;
  rbi.fMask = mask;
  rbi.fStyle = style;
  rbi.hwndChild = hwnd_child;

  return SendMessage(RB_INSERTBAND, -1, reinterpret_cast<LPARAM>(&rbi));
}

}  // namespace win