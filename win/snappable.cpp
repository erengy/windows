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

#include <windows.h>
#include <dwmapi.h>

#include "gdi.h"
#include "snappable.h"

namespace win {

void Snappable::SetSnapGap(int snap_gap) {
  snap_gap_ = snap_gap;
}

BOOL Snappable::SnapProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_ENTERSIZEMOVE: {
      moving_ = true;
      if (snapped_) {
        // Save the cursor offset from the window borders, so the dialog window
        // can be unsnapped later
        RECT rc;
        POINT cursor;
        if (::GetWindowRect(hwnd, &rc) && ::GetCursorPos(&cursor)) {
          snap_dx_ = cursor.x - rc.left;
          snap_dy_ = cursor.y - rc.top;
        }
      }
      break;
    }
    case WM_EXITSIZEMOVE: {
      moving_ = false;
      break;
    }
    case WM_MOVING: {
      moving_ = true;
      if (SnapToEdges(hwnd, reinterpret_cast<LPRECT>(lParam)))
        return TRUE;
      break;
    }
    case WM_SIZE: {
      if (moving_)
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

  Rect rect;
  POINT cursor;
  if (!::GetWindowRect(hwnd, &rect) || !::GetCursorPos(&cursor))
    return false;

  // Check if window is going to be aero-snapped
  if ((rc->right - rc->left != rect.Width()) ||
      (rc->bottom - rc->top != rect.Height()))
    return false;

  // Check if window has already been aero-snapped
  WINDOWPLACEMENT wp = {0};
  wp.length = sizeof(wp);
  if (!::GetWindowPlacement(hwnd, &wp))
    return false;
  Rect wpRect(wp.rcNormalPosition);
  if ((rc->right - rc->left != wpRect.Width()) ||
      (rc->bottom - rc->top != wpRect.Height()))
    return false;

  RECT wr = {0};
  ::SystemParametersInfo(SPI_GETWORKAREA, 0, &wr, 0);
  if (::GetSystemMetrics(SM_CMONITORS) > 1) {
    HMONITOR monitor = ::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    if (monitor) {
      MONITORINFO mi = {0};
      mi.cbSize = sizeof(mi);
      ::GetMonitorInfo(monitor, &mi);
      wr = mi.rcWork;
    }
  }

  // Check for invisible borders and adjust the work area size
  RECT frame = {0};
  if (::DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS,
                              &frame, sizeof(RECT)) == S_OK) {
    wr.left -= frame.left - rect.left;
    wr.top -= frame.top - rect.top;
    wr.right += rect.right - frame.right;
    wr.bottom += rect.bottom - frame.bottom;
  }

  // Let the window to unsnap by changing its position, otherwise it will stick
  // to the screen edges forever
  rect.Copy(*rc);
  if (snapped_) {
    rect.Offset(cursor.x - rect.left - snap_dx_,
                cursor.y - rect.top - snap_dy_);
  }

  bool snapped = false;
  // Snap X axis
  if (abs(rect.left - wr.left) < snap_gap_) {
    snapped = true;
    rect.Offset(wr.left - rect.left, 0);
  } else if (abs(rect.right - wr.right) < snap_gap_) {
    snapped = true;
    rect.Offset(wr.right - rect.right, 0);
  }
  // Snap Y axis
  if (abs(rect.top - wr.top) < snap_gap_) {
    snapped = true;
    rect.Offset(0, wr.top - rect.top);
  } else if (abs(rect.bottom - wr.bottom) < snap_gap_) {
    snapped = true;
    rect.Offset(0, wr.bottom - rect.bottom);
  }

  if (!snapped_ && snapped) {
    snap_dx_ = cursor.x - rc->left;
    snap_dy_ = cursor.y - rc->top;
  }

  snapped_ = snapped;
  ::CopyRect(rc, &rect);

  return true;
}

}  // namespace win
