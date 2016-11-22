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

#define NOMINMAX

#include <algorithm>

#include "gdi.h"
#include "resizable.h"

// see: http://support.microsoft.com/kb/262954

namespace win {

Resizable::Resizable()
    : horizontal_(true), vertical_(true),
      x_(1), y_(1) {
}

Resizable::Resizable(bool horizontal, bool vertical)
    : horizontal_(horizontal), vertical_(vertical),
      x_(1), y_(1) {
}

void Resizable::ResizeProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_INITDIALOG:
      OnInitDialog(hwnd);
      break;
    case WM_HSCROLL:
      OnScroll(hwnd, SB_HORZ, LOWORD(wParam));
      break;
    case WM_VSCROLL:
      OnScroll(hwnd, SB_VERT, LOWORD(wParam));
      break;
    case WM_SIZE: {
      SIZE size = {LOWORD(lParam), HIWORD(lParam)};
      OnSize(hwnd, static_cast<UINT>(wParam), size);
      break;
    }
  }
}

void Resizable::OnInitDialog(HWND hwnd) {
  Rect rect;
  ::GetClientRect(hwnd, &rect);

  SCROLLINFO si = {0};
  si.cbSize = sizeof(SCROLLINFO);
  si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
  si.nMin = 1;
  si.nPos = 1;

  if (horizontal_) {
    si.nMax = rect.Width();
    si.nPage = rect.Width();
    ::SetScrollInfo(hwnd, SB_HORZ, &si, FALSE);
  }
  if (vertical_) {
    si.nMax = rect.Height();
    si.nPage = rect.Height();
    ::SetScrollInfo(hwnd, SB_VERT, &si, FALSE);
  }
}

void Resizable::OnScroll(HWND hwnd, int bar, UINT code) {
  SCROLLINFO si = {0};
  si.cbSize = sizeof(SCROLLINFO);
  si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE | SIF_TRACKPOS;
  ::GetScrollInfo(hwnd, bar, &si);

  int min_pos = si.nMin;
  int max_pos = si.nMax - (si.nPage - 1);
  int pos = -1;

  switch (code) {
    case SB_LINEUP:
      pos = std::max(si.nPos - 1, min_pos);
      break;
    case SB_LINEDOWN:
      pos = std::min(si.nPos + 1, max_pos);
      break;
    case SB_PAGEUP:
      pos = std::max(si.nPos - (int)si.nPage, min_pos);
      break;
    case SB_PAGEDOWN:
      pos = std::min(si.nPos + (int)si.nPage, max_pos);
      break;
    case SB_THUMBPOSITION:
      break;
    case SB_THUMBTRACK:
      pos = si.nTrackPos;
      break;
    case SB_TOP:
      pos = min_pos;
      break;
    case SB_BOTTOM:
      pos = max_pos;
      break;
    case SB_ENDSCROLL:
      break;
    default:
      return;
  }

  if (pos == -1)
    return;

  ::SetScrollPos(hwnd, bar, pos, TRUE);
  ScrollClient(hwnd, bar, pos);
}

void Resizable::OnSize(HWND hwnd, UINT type, SIZE size) {
  if (type != SIZE_RESTORED && type != SIZE_MAXIMIZED)
    return;

  SCROLLINFO si = {0};
  si.cbSize = sizeof(SCROLLINFO);

  const int bar[] = { SB_HORZ, SB_VERT };
  const int page[] = { size.cx, size.cy };

  for (size_t i = 0; i < ARRAYSIZE(bar); i++) {
    si.fMask = SIF_PAGE;
    si.nPage = page[i];
    ::SetScrollInfo(hwnd, bar[i], &si, TRUE);

    si.fMask = SIF_RANGE | SIF_POS;
    ::GetScrollInfo(hwnd, bar[i], &si);

    const int max_pos = si.nMax - (page[i] - 1);

    if ((si.nPos != si.nMin && si.nPos == max_pos) ||
        (type == SIZE_MAXIMIZED))
      ScrollClient(hwnd, bar[i], si.nPos);
  }
}

void Resizable::ScrollClient(HWND hwnd, int bar, int pos) {
  int x = 0;
  int y = 0;

  int& delta = bar == SB_HORZ ? x : y;
  int& prev = bar == SB_HORZ ? x_ : y_;

  delta = prev - pos;
  prev = pos;

  if (x || y)
    ::ScrollWindow(hwnd, x, y, nullptr, nullptr);
}

}  // namespace win