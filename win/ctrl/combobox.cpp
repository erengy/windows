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

#include <windowsx.h>

#include "../common_controls.h"

namespace win {

ComboBox::ComboBox(HWND hwnd) {
  SetWindowHandle(hwnd);
}

void ComboBox::PreCreate(CREATESTRUCT &cs) {
  cs.dwExStyle = WS_EX_CLIENTEDGE;
  cs.lpszClass = WC_COMBOBOX;
  cs.style = CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_TABSTOP | WS_VISIBLE;
}

void ComboBox::OnCreate(HWND hwnd, LPCREATESTRUCT create_struct) {
  Window::OnCreate(hwnd, create_struct);
}

////////////////////////////////////////////////////////////////////////////////

int ComboBox::AddItem(LPCWSTR text, LPARAM data) {
  int index = ComboBox_AddString(window_, text);
  return ComboBox_SetItemData(window_, index, data);
}

int ComboBox::AddString(LPCWSTR text) {
  return ComboBox_AddString(window_, text);
}

int ComboBox::DeleteString(int index) {
  return ComboBox_DeleteString(window_, index);
}

int ComboBox::FindItemData(LPARAM data) {
  for (int i = 0; i < GetCount(); i++)
    if (data == GetItemData(i))
      return i;

  return CB_ERR;
}

int ComboBox::GetCount() {
  return ComboBox_GetCount(window_);
}

int ComboBox::GetCurSel() {
  return ComboBox_GetCurSel(window_);
}

LRESULT ComboBox::GetItemData(int index) {
  return ComboBox_GetItemData(window_, index);
}

void ComboBox::ResetContent() {
  ComboBox_ResetContent(window_);
}

BOOL ComboBox::SetCueBannerText(LPCWSTR text) {
  return ComboBox_SetCueBannerText(window_, text);
}

int ComboBox::SetCurSel(int index) {
  return ComboBox_SetCurSel(window_, index);
}

BOOL ComboBox::SetEditSel(int start, int end) {
  return ::SendMessage(window_, CB_SETEDITSEL, start, end);
}

int ComboBox::SetItemData(int index, LPARAM data) {
  return ComboBox_SetItemData(window_, index, data);
}

}  // namespace win