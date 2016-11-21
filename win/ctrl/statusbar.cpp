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

StatusBar::StatusBar(HWND hwnd) {
  SetWindowHandle(hwnd);
}

void StatusBar::PreCreate(CREATESTRUCT &cs) {
  cs.dwExStyle = 0;
  cs.lpszClass = STATUSCLASSNAME;
  cs.style = WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP | SBARS_TOOLTIPS;
}

void StatusBar::OnCreate(HWND hwnd, LPCREATESTRUCT create_struct) {
  widths_.clear();
  Window::OnCreate(hwnd, create_struct);
}

////////////////////////////////////////////////////////////////////////////////

void StatusBar::GetRect(int part, LPRECT rect) {
  SendMessage(SB_GETRECT, part, reinterpret_cast<LPARAM>(rect));
}

int StatusBar::InsertPart(int image, int style, int autosize, int width,
                          LPCWSTR text, LPCWSTR tooltip) {
  if (widths_.empty()) {
    widths_.push_back(width);
  } else {
    widths_.push_back(widths_.back() + width);
  }

  int part_count = SendMessage(SB_GETPARTS, 0, 0);

  SendMessage(SB_SETPARTS, part_count + 1,
      reinterpret_cast<LPARAM>(&widths_[0]));
  SendMessage(SB_SETTEXT, part_count - 1,
      reinterpret_cast<LPARAM>(text));
  SendMessage(SB_SETTIPTEXT, part_count - 1,
      reinterpret_cast<LPARAM>(tooltip));

  if (image > -1 && image_list_) {
    HICON icon = ::ImageList_GetIcon(image_list_, image, 0);
    SendMessage(SB_SETICON, part_count - 1, reinterpret_cast<LPARAM>(icon));
  }

  return part_count;
}

void StatusBar::SetImageList(HIMAGELIST image_list) {
  image_list_ = image_list;
}

void StatusBar::SetPartText(int part, LPCWSTR text) {
  SendMessage(SB_SETTEXT, part, reinterpret_cast<LPARAM>(text));
}

void StatusBar::SetPartTipText(int part, LPCWSTR tip_text) {
  SendMessage(SB_SETTIPTEXT, part, reinterpret_cast<LPARAM>(tip_text));
}

void StatusBar::SetPartWidth(int part, int width) {
  if (part > static_cast<int>(widths_.size()) - 1)
    return;

  if (part == 0) {
    widths_.at(part) = width;
  } else {
    widths_.at(part) = widths_.at(part - 1) + width;
  }

  SendMessage(SB_SETPARTS, widths_.size(),
              reinterpret_cast<LPARAM>(&widths_[0]));
}

}  // namespace win