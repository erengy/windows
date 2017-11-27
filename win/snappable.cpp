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

#include <cstdlib>

#include <windows.h>
#include <dwmapi.h>

#include "gdi.h"
#include "snappable.h"

namespace win {

POINT GetCursorOffset(const RECT& rc) {
  POINT cursor = {0};
  if (::GetCursorPos(&cursor)) {
    cursor.x -= rc.left;
    cursor.y -= rc.top;
  }
  return cursor;
}

Rect GetWorkArea(HWND hwnd, const Rect& rc_window) {
  RECT rc = {0};
  ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);

  if (::GetSystemMetrics(SM_CMONITORS) > 1) {
    HMONITOR monitor = ::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    if (monitor) {
      MONITORINFO mi = {0};
      mi.cbSize = sizeof(mi);
      if (::GetMonitorInfo(monitor, &mi))
        rc = mi.rcWork;
    }
  }

  // Check for invisible borders
  RECT frame = {0};
  if (::DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS,
                              &frame, sizeof(RECT)) == S_OK) {
    rc.left -= frame.left - rc_window.left;
    rc.top -= frame.top - rc_window.top;
    rc.right += rc_window.right - frame.right;
    rc.bottom += rc_window.bottom - frame.bottom;
  }

  return rc;
}

bool IsAeroSnapped(HWND hwnd, const Rect& rc, const Rect& rc_window) {
  const auto equal_rect_dimensions = [](const Rect& a, const Rect& b) {
    return a.Width() == b.Width() && a.Height() == b.Height();
  };

  // Check if window is going to be aero-snapped
  if (!equal_rect_dimensions(rc, rc_window))
    return true;

  // Check if window has already been aero-snapped
  WINDOWPLACEMENT wp = {0};
  wp.length = sizeof(wp);
  if (::GetWindowPlacement(hwnd, &wp)) {
    if (!equal_rect_dimensions(rc, wp.rcNormalPosition))
      return true;
  }

  return false;
}

bool SnapRect(const Rect& rc_work, const int gap, Rect& rc) {
  const RECT d{
    rc_work.left - rc.left,
    rc_work.top - rc.top,
    rc_work.right - rc.right,
    rc_work.bottom - rc.bottom
  };

  const auto dx = std::abs(d.left) < gap ? d.left :
                  std::abs(d.right) < gap ? d.right : 0;
  const auto dy = std::abs(d.top) < gap ? d.top :
                  std::abs(d.bottom) < gap ? d.bottom : 0;

  if (dx || dy) {
    rc.Offset(dx, dy);
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////

void Snappable::SetSnapGap(int snap_gap) {
  snap_gap_ = snap_gap;
}

BOOL Snappable::SnapProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  static bool moving = false;

  switch (uMsg) {
    case WM_ENTERSIZEMOVE: {
      moving = true;
      if (snapped_) {
        // Save the cursor offset from the window borders, so the window can be
        // unsnapped later
        RECT rc;
        if (::GetWindowRect(hwnd, &rc))
          snap_offset_ = GetCursorOffset(rc);
      }
      break;
    }
    case WM_EXITSIZEMOVE: {
      moving = false;
      break;
    }
    case WM_MOVING: {
      moving = true;
      if (SnapToEdges(hwnd, reinterpret_cast<LPRECT>(lParam)))
        return TRUE;
      break;
    }
    case WM_SIZE: {
      if (moving)
        snapped_ = false;
      break;
    }
  }

  return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

bool Snappable::SnapToEdges(HWND hwnd, LPRECT rc) {
  if (!snap_gap_) {
    snapped_ = false;
    return false;
  }

  Rect rc_window;
  if (!::GetWindowRect(hwnd, &rc_window))
    return false;

  if (IsAeroSnapped(hwnd, rc, rc_window))
    return false;

  const auto offset = GetCursorOffset(*rc);
  const auto rc_work = GetWorkArea(hwnd, rc_window);
  Rect rect = *rc;

  // Let the window unsnap by changing its position, otherwise it will stick to
  // the screen edges forever
  if (snapped_)
    rect.Offset(offset.x - snap_offset_.x, offset.y - snap_offset_.y);

  const auto snapped = SnapRect(rc_work, snap_gap_, rect);

  if (!snapped_ && snapped)
    snap_offset_ = offset;
  snapped_ = snapped;

  ::CopyRect(rc, &rect);

  return true;
}

}  // namespace win
