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

#include "../common_controls.h"

namespace win {

Tab::Tab(HWND hwnd) {
  SetWindowHandle(hwnd);
}

void Tab::PreCreate(CREATESTRUCT &cs) {
  cs.dwExStyle = 0;
  cs.lpszClass = WC_TABCONTROL;
  cs.style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | TCS_TOOLTIPS;
}

void Tab::OnCreate(HWND hwnd, LPCREATESTRUCT create_struct) {
  TabCtrl_SetExtendedStyle(hwnd, TCS_EX_REGISTERDROP);
  Window::OnCreate(hwnd, create_struct);
}

////////////////////////////////////////////////////////////////////////////////

void Tab::AdjustRect(HWND hwnd, BOOL larger, LPRECT rect) {
  if (hwnd) {
    RECT window_rect;
    ::GetClientRect(window_, rect);
    ::GetWindowRect(window_, &window_rect);
    ::ScreenToClient(hwnd, (LPPOINT)&window_rect);
    ::SetRect(rect,
              window_rect.left,
              window_rect.top,
              window_rect.left + rect->right,
              window_rect.top + rect->bottom);
  }

  TabCtrl_AdjustRect(window_, larger, rect);
}

int Tab::DeleteAllItems() {
  return TabCtrl_DeleteAllItems(window_);
}

int Tab::DeleteItem(int index) {
  return TabCtrl_DeleteItem(window_, index);
}

int Tab::InsertItem(int index, LPCWSTR text, LPARAM param) {
  TCITEM tci;

  tci.mask = TCIF_PARAM | TCIF_TEXT;
  tci.pszText = const_cast<LPWSTR>(text);
  tci.lParam = param;
  tci.iImage = -1;

  return TabCtrl_InsertItem(window_, index, &tci);
}

int Tab::GetCurrentlySelected() {
  return TabCtrl_GetCurSel(window_);
}

int Tab::GetItemCount() {
  return TabCtrl_GetItemCount(window_);
}

LPARAM Tab::GetItemParam(int index) {
  TCITEM tci;
  tci.mask = TCIF_PARAM;
  TabCtrl_GetItem(window_, index, &tci);

  return tci.lParam;
}

int Tab::HitTest() {
  TCHITTESTINFO tchti;
  ::GetCursorPos(&tchti.pt);
  ::ScreenToClient(window_, &tchti.pt);

  return TabCtrl_HitTest(window_, &tchti);
}

int Tab::SetCurrentlySelected(int item) {
  return TabCtrl_SetCurSel(window_, item);
}

int Tab::SetItemText(int item, LPCWSTR text) {
  TCITEM tci;
  tci.mask = TCIF_TEXT;
  tci.pszText = const_cast<LPWSTR>(text);

  return TabCtrl_SetItem(window_, item, &tci);
}

}  // namespace win