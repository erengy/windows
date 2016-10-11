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

#pragma once

#include "win_main.h"

namespace win {

enum WindowBorderStyle {
  kWindowBorderNone,
  kWindowBorderClient,
  kWindowBorderStatic
};

class Window {
public:
  Window();
  Window(HWND hwnd);
  virtual ~Window();

  virtual HWND Create(HWND parent = nullptr);
  virtual HWND Create(DWORD ex_style, LPCWSTR class_name, LPCWSTR window_name, DWORD style, int x, int y, int width, int height, HWND parent, HMENU menu, LPVOID param);
  virtual void Destroy();
  virtual void PreCreate(CREATESTRUCT& cs);
  virtual void PreRegisterClass(WNDCLASSEX& wc);
  virtual BOOL PreTranslateMessage(MSG* msg);

  void    Attach(HWND hwnd);
  void    CenterOwner();
  HWND    Detach();
  LPCWSTR GetClassName() const;
  HMENU   GetMenuHandle() const;
  HWND    GetParentHandle() const;
  HICON   SetIconLarge(HICON icon);
  HICON   SetIconLarge(int icon);
  HICON   SetIconSmall(HICON icon);
  HICON   SetIconSmall(int icon);
  HWND    GetWindowHandle() const;
  void    SetWindowHandle(HWND hwnd);

  // Win32 API wrappers
  BOOL    BringWindowToTop() const;
  BOOL    Close() const;
  BOOL    Enable(BOOL enable = TRUE) const;
  BOOL    GetClientRect(LPRECT rect) const;
  HDC     GetDC() const;
  HFONT   GetFont() const;
  HMENU   GetMenu() const;
  HWND    GetParent();
  BOOL    GetPlacement(WINDOWPLACEMENT& wp) const;
  void    GetText(LPWSTR output, int max_count = MAX_PATH) const;
  void    GetText(std::wstring& output) const;
  std::wstring GetText() const;
  INT     GetTextLength() const;
  DWORD   GetWindowLong(int index = GWL_STYLE) const;
  BOOL    GetWindowRect(LPRECT rect) const;
  void    GetWindowRect(HWND hwnd_to, LPRECT rect) const;
  BOOL    Hide() const;
  BOOL    HideCaret() const;
  BOOL    InvalidateRect(LPCRECT rect = nullptr, BOOL erase = TRUE) const;
  BOOL    IsEnabled() const;
  BOOL    IsIconic() const;
  BOOL    IsVisible() const;
  BOOL    IsWindow() const;
  INT     MessageBox(LPCWSTR text, LPCWSTR caption, UINT type) const;
  LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;
  BOOL    RedrawWindow(LPCRECT rect = nullptr, HRGN region = nullptr, UINT flags = RDW_INVALIDATE | RDW_ALLCHILDREN) const;
  LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;
  BOOL    SetBorder(int style) const;
  HWND    SetCapture() const;
  DWORD   SetClassLong(int index, LONG new_long) const;
  HWND    SetFocus() const;
  void    SetFont(LPCWSTR face_name, int size, bool bold = false, bool italic = false, bool underline = false);
  void    SetFont(HFONT font);
  BOOL    SetForegroundWindow() const;
  BOOL    SetMenu(HMENU menu) const;
  void    SetParent(HWND parent) const;
  BOOL    SetPlacement(const WINDOWPLACEMENT& wp) const;
  BOOL    SetPosition(HWND hwnd_insert_after, int x, int y, int w, int h, UINT flags = SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER) const;
  BOOL    SetPosition(HWND hwnd_insert_after, const RECT& rc, UINT flags = SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER) const;
  BOOL    SetRedraw(BOOL redraw) const;
  void    SetStyle(UINT style, UINT style_not, int index = GWL_STYLE) const;
  BOOL    SetText(LPCWSTR text) const;
  BOOL    SetText(const std::wstring& text) const;
  HRESULT SetTheme(LPCWSTR theme_name = L"explorer") const;
  BOOL    SetTransparency(BYTE alpha, COLORREF color = 0xFF000000) const;
  BOOL    Show(int cmd_show = SW_SHOWNORMAL) const;
  BOOL    Update() const;

protected:
  // Message handlers
  virtual BOOL    OnCommand(WPARAM wParam, LPARAM lParam) { return FALSE; }
  virtual void    OnContextMenu(HWND hwnd, POINT pt) {}
  virtual void    OnCreate(HWND hwnd, LPCREATESTRUCT create_struct);
  virtual BOOL    OnDestroy() { return FALSE; }
  virtual void    OnDropFiles(HDROP drop_info) {}
  virtual void    OnGetMinMaxInfo(LPMINMAXINFO mmi) {}
  virtual LRESULT OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) { return -1; }
  virtual void    OnMove(LPPOINTS pts) {}
  virtual LRESULT OnNotify(int control_id, LPNMHDR nmh) { return 0; }
  virtual void    OnPaint(HDC hdc, LPPAINTSTRUCT ps) {}
  virtual void    OnSize(UINT uMsg, UINT type, SIZE size) {}
  virtual void    OnTaskbarCallback(UINT uMsg, LPARAM lParam) {}
  virtual void    OnTimer(UINT_PTR event_id) {}
  virtual void    OnWindowPosChanging(LPWINDOWPOS window_pos) {}
  virtual LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  virtual LRESULT WindowProcDefault(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  CREATESTRUCT create_struct_;
  WNDCLASSEX   window_class_;
  HINSTANCE    instance_;
  HFONT        font_;
  HICON        icon_large_, icon_small_;
  HMENU        menu_;
  HWND         parent_, window_;
  WNDPROC      prev_window_proc_;

private:
  static LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  BOOL RegisterClass(WNDCLASSEX& wc) const;
  void Subclass(HWND hwnd);
  void UnSubclass();

  static Window* current_window_;
};

}  // namespace win
