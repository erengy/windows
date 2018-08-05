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

RichEdit::RichEdit() {
  module_msftedit_ = ::LoadLibrary(L"msftedit.dll");
  module_riched20_ = ::LoadLibrary(L"riched20.dll");
}

RichEdit::RichEdit(HWND hwnd) {
  module_msftedit_ = ::LoadLibrary(L"msftedit.dll");
  module_riched20_ = ::LoadLibrary(L"riched20.dll");
  SetWindowHandle(hwnd);
}

RichEdit::~RichEdit() {
  if (module_msftedit_)
    ::FreeLibrary(module_msftedit_);
  if (module_riched20_)
    ::FreeLibrary(module_riched20_);
}

void RichEdit::PreCreate(CREATESTRUCT &cs) {
  cs.dwExStyle = WS_EX_STATICEDGE;
  cs.lpszClass = RICHEDIT_CLASS;
  cs.style = WS_CHILD | WS_CLIPCHILDREN | WS_TABSTOP | WS_VISIBLE | WS_VSCROLL |
             ES_AUTOHSCROLL;
}

void RichEdit::OnCreate(HWND hwnd, LPCREATESTRUCT create_struct) {
  Window::OnCreate(hwnd, create_struct);
}

////////////////////////////////////////////////////////////////////////////////

void RichEdit::GetSel(CHARRANGE* cr) {
  SendMessage(EM_EXGETSEL, 0, reinterpret_cast<LPARAM>(cr));
}

std::wstring RichEdit::GetTextRange(CHARRANGE* cr) {
  std::wstring text(1024, L'\0');

  TEXTRANGE tr;
  tr.chrg.cpMax = cr->cpMax;
  tr.chrg.cpMin = cr->cpMin;
  tr.lpstrText = const_cast<LPWSTR>(text.data());

  const auto size = SendMessage(
      EM_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr));
  text.resize(size);

  return text;
}

void RichEdit::HideSelection(BOOL hide) {
  SendMessage(EM_HIDESELECTION, hide, 0);
}

BOOL RichEdit::SetCharFormat(DWORD format, CHARFORMAT* cf) {
  return SendMessage(EM_SETCHARFORMAT, format, reinterpret_cast<LPARAM>(cf));
}

DWORD RichEdit::SetEventMask(DWORD flags) {
  return SendMessage(EM_SETEVENTMASK, 0, flags);
}

void RichEdit::SetSel(int start, int end) {
  SendMessage(EM_SETSEL, start, end);
}

void RichEdit::SetSel(CHARRANGE* cr) {
  SendMessage(EM_EXSETSEL, 0, reinterpret_cast<LPARAM>(cr));
}

UINT RichEdit::SetTextEx(const std::string& text) {
  SETTEXTEX ste;
  ste.codepage = CP_UTF8;
  ste.flags = ST_DEFAULT;

  return SendMessage(EM_SETTEXTEX,
                     reinterpret_cast<WPARAM>(&ste),
                     reinterpret_cast<LPARAM>(text.data()));
}

}  // namespace win