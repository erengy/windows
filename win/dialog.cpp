/*
MIT License

Copyright (c) 2010-2021 Eren Okka

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

#include <vector>

#include <windowsx.h>

#include "dialog.h"
#include "taskbar.h"
#include "window_map.h"

namespace win {

Dialog::Dialog()
    : modal_(true) {
  size_last_.cx = 0;
  size_last_.cy = 0;

  size_max_.cx = 0;
  size_max_.cy = 0;

  size_min_.cx = 0;
  size_min_.cy = 0;

  pos_last_.x = 0;
  pos_last_.y = 0;
}

Dialog::~Dialog() {
  EndDialog(0);
}

INT_PTR Dialog::Create(UINT resource_id, HWND parent, bool modal) {
  modal_ = modal;

  if (modal) {
    INT_PTR result = ::DialogBoxParam(
        instance_, MAKEINTRESOURCE(resource_id), parent, DialogProcStatic,
        reinterpret_cast<LPARAM>(this));
    window_ = nullptr;
    return result;

  } else {
    window_ = ::CreateDialogParam(
        instance_, MAKEINTRESOURCE(resource_id), parent, DialogProcStatic,
        reinterpret_cast<LPARAM>(this));
    return reinterpret_cast<INT_PTR>(window_);
  }
}

void Dialog::EndDialog(INT_PTR result) {
#ifdef WIN_DIALOG_REMEMBER_LAST_POSITION_AND_SIZE
  RECT rect;
  GetWindowRect(&rect);
  pos_last_.x = rect.left;
  pos_last_.y = rect.top;
  size_last_.cx = rect.right - rect.left;
  size_last_.cy = rect.bottom - rect.top;
#endif

  if (IsWindow()) {
    if (modal_) {
      ::EndDialog(window_, result);
    } else {
      Destroy();
    }
  }
  window_ = nullptr;
}

void Dialog::SetSizeMax(LONG cx, LONG cy) {
  size_max_.cx = cx;
  size_max_.cy = cy;
}

void Dialog::SetSizeMin(LONG cx, LONG cy) {
  size_min_.cx = cx;
  size_min_.cy = cy;
}

////////////////////////////////////////////////////////////////////////////////

void Dialog::SetMinMaxInfo(LPMINMAXINFO mmi) {
  if (size_max_.cx > 0)
    mmi->ptMaxTrackSize.x = size_max_.cx;
  if (size_max_.cy > 0)
    mmi->ptMaxTrackSize.y = size_max_.cy;

  if (size_min_.cx > 0)
    mmi->ptMinTrackSize.x = size_min_.cx;
  if (size_min_.cy > 0)
    mmi->ptMinTrackSize.y = size_min_.cy;
}

////////////////////////////////////////////////////////////////////////////////

void Dialog::OnCancel() {
  EndDialog(IDCANCEL);
}

BOOL Dialog::OnClose() {
  return FALSE;
}

BOOL Dialog::OnInitDialog() {
  return TRUE;
}

void Dialog::OnOK() {
  EndDialog(IDOK);
}

// Superclassing
void Dialog::RegisterDlgClass(LPCWSTR class_name) {
  WNDCLASS wc;
  ::GetClassInfo(instance_, WC_DIALOG, &wc);  // WC_DIALOG is #32770
  wc.lpszClassName = class_name;
  ::RegisterClass(&wc);
}

////////////////////////////////////////////////////////////////////////////////

LRESULT Dialog::AddComboString(int id_combo, LPCWSTR text) {
  return SendDlgItemMessage(id_combo, CB_ADDSTRING, 0,
                            reinterpret_cast<LPARAM>(text));
}

BOOL Dialog::CheckDlgButton(int id_button, UINT check) {
  return ::CheckDlgButton(window_, id_button, check);
}

BOOL Dialog::CheckRadioButton(int id_first_button, int id_last_button,
                              int id_check_button) {
  return ::CheckRadioButton(window_, id_first_button, id_last_button,
                            id_check_button);
}

BOOL Dialog::EnableDlgItem(int id_item, BOOL enable) {
  return ::EnableWindow(GetDlgItem(id_item), enable);
}

INT Dialog::GetCheckedRadioButton(int id_first_button, int id_last_button) {
  for (int i = 0; i <= id_last_button - id_first_button; i++)
    if (IsDlgButtonChecked(id_first_button + i))
      return i;

  return 0;
}

LRESULT Dialog::GetComboSelection(int id_item) {
  return SendDlgItemMessage(id_item, CB_GETCURSEL, 0, 0);
}

HWND Dialog::GetDlgItem(int id_item) {
  return ::GetDlgItem(window_, id_item);
}

UINT Dialog::GetDlgItemInt(int id_item) {
  return ::GetDlgItemInt(window_, id_item, nullptr, TRUE);
}

void Dialog::GetDlgItemText(int id_item, LPWSTR output, int max_length) {
  ::GetDlgItemText(window_, id_item, output, max_length);
}

void Dialog::GetDlgItemText(int id_item, std::wstring& output) {
  int len = ::GetWindowTextLength(GetDlgItem(id_item)) + 1;
  std::vector<wchar_t> buffer(len);
  ::GetDlgItemText(window_, id_item, &buffer[0], len);
  output.assign(&buffer[0]);
}

std::wstring Dialog::GetDlgItemText(int id_item) {
  int len = ::GetWindowTextLength(GetDlgItem(id_item)) + 1;
  std::vector<wchar_t> buffer(len);
  ::GetDlgItemText(window_, id_item, &buffer[0], len);
  return std::wstring(&buffer[0]);
}

BOOL Dialog::HideDlgItem(int id_item) {
  return ::ShowWindow(GetDlgItem(id_item), SW_HIDE);
}

BOOL Dialog::IsDlgButtonChecked(int id_button) {
  return ::IsDlgButtonChecked(window_, id_button);
}

bool Dialog::IsModal() {
  return modal_;
}

LRESULT Dialog::SendDlgItemMessage(int id_item, UINT uMsg,
                                   WPARAM wParam, LPARAM lParam) {
  return ::SendDlgItemMessage(window_, id_item, uMsg, wParam, lParam);
}

LRESULT Dialog::SetComboSelection(int id_item, int index) {
  return SendDlgItemMessage(id_item, CB_SETCURSEL, index, 0);
}

BOOL Dialog::SetDlgItemText(int id_item, LPCWSTR text) {
  return ::SetDlgItemText(window_, id_item, text);
}

BOOL Dialog::ShowDlgItem(int id_item, int cmd_show) {
  return ::ShowWindow(GetDlgItem(id_item), cmd_show);
}

////////////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK Dialog::DialogProcStatic(HWND hwnd, UINT uMsg,
                                          WPARAM wParam, LPARAM lParam) {
  Dialog* window = reinterpret_cast<Dialog*>(WindowMap::Instance().GetWindow(hwnd));

  if (!window && uMsg == WM_INITDIALOG) {
    window = reinterpret_cast<Dialog*>(lParam);
    if (window) {
      window->SetWindowHandle(hwnd);
      WindowMap::Instance().Add(hwnd, window);
    }
  }

  if (window) {
    return window->DialogProc(hwnd, uMsg, wParam, lParam);
  } else {
    return FALSE;
  }
}

INT_PTR Dialog::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  return DialogProcDefault(hwnd, uMsg, wParam, lParam);
}

INT_PTR Dialog::DialogProcDefault(HWND hwnd, UINT uMsg,
                                  WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_CLOSE: {
      return OnClose();
    }
    case WM_COMMAND: {
      switch (LOWORD(wParam)) {
        case IDOK:
          OnOK();
          return TRUE;
        case IDCANCEL:
          OnCancel();
          return TRUE;
        default:
          return OnCommand(wParam, lParam);
      }
      break;
    }
    case WM_CONTEXTMENU: {
      POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
      OnContextMenu(reinterpret_cast<HWND>(wParam), pt);
      break;
    }
    case WM_DESTROY: {
      OnDestroy();
      break;
    }
    case WM_INITDIALOG: {
#ifdef WIN_DIALOG_REMEMBER_LAST_POSITION_AND_SIZE
      if (pos_last_.x && pos_last_.y)
        SetPosition(nullptr, pos_last_.x, pos_last_.y, 0, 0, SWP_NOSIZE);
      if (size_last_.cx && size_last_.cy)
        SetPosition(nullptr, 0, 0, size_last_.cx, size_last_.cy, SWP_NOMOVE);
#endif
      return OnInitDialog();
    }
    case WM_DROPFILES: {
      OnDropFiles(reinterpret_cast<HDROP>(wParam));
      break;
    }
    case WM_ENTERSIZEMOVE:
    case WM_EXITSIZEMOVE: {
      SIZE size = {0};
      OnSize(uMsg, 0, size);
      break;
    }
    case WM_GETMINMAXINFO: {
      LPMINMAXINFO mmi = reinterpret_cast<LPMINMAXINFO>(lParam);
      SetMinMaxInfo(mmi);
      OnGetMinMaxInfo(mmi);
      break;
    }
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MOUSEACTIVATE:
    case WM_MOUSEHOVER:
    case WM_MOUSEHWHEEL:
    case WM_MOUSELEAVE:
    case WM_MOUSEMOVE:
    case WM_MOUSEWHEEL: {
      LRESULT result = OnMouseEvent(uMsg, wParam, lParam);
      if (result != -1) {
        ::SetWindowLongPtrW(hwnd, DWLP_MSGRESULT, result);
        return TRUE;
      }
      break;
    }
    case WM_MOVE: {
      POINTS pts = MAKEPOINTS(lParam);
      OnMove(&pts);
      break;
    }
    case WM_NOTIFY: {
      LRESULT result = OnNotify(static_cast<int>(wParam),
                                reinterpret_cast<LPNMHDR>(lParam));
      if (result) {
        ::SetWindowLongPtr(hwnd, DWLP_MSGRESULT, result);
        return TRUE;
      }
      break;
    }
    case WM_PAINT: {
      if (::GetUpdateRect(hwnd, nullptr, FALSE)) {
        PAINTSTRUCT ps;
        HDC hdc = ::BeginPaint(hwnd, &ps);
        OnPaint(hdc, &ps);
        ::EndPaint(hwnd, &ps);
      } else {
        HDC hdc = ::GetDC(hwnd);
        OnPaint(hdc, nullptr);
        ::ReleaseDC(hwnd, hdc);
      }
      break;
    }
    case WM_SIZE: {
      SIZE size = {LOWORD(lParam), HIWORD(lParam)};
      OnSize(uMsg, static_cast<UINT>(wParam), size);
      break;
    }
    case WM_TIMER: {
      OnTimer(static_cast<UINT_PTR>(wParam));
      break;
    }
    case WM_WINDOWPOSCHANGING: {
      OnWindowPosChanging(reinterpret_cast<LPWINDOWPOS>(lParam));
      break;
    }
    default: {
      if (uMsg == WM_TASKBARCREATED ||
          uMsg == WM_TASKBARBUTTONCREATED ||
          uMsg == WM_TASKBARCALLBACK) {
        OnTaskbarCallback(uMsg, lParam);
        return FALSE;
      }
      break;
    }
  }

  return FALSE;
}

}  // namespace win
