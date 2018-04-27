/*
MIT License

Copyright (c) 2010-2018 Eren Okka

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

#include <string>

#include <windows.h>

namespace win {

class Clipboard {
public:
  Clipboard(HWND hwnd_new_owner) {
    Open(hwnd_new_owner);
  }

  ~Clipboard() {
    Close();
  }

  bool Open(HWND hwnd_new_owner) const {
    return ::OpenClipboard(hwnd_new_owner);
  }

  bool Close() const {
    return ::CloseClipboard();
  }

  bool Empty() const {
    return ::EmptyClipboard();
  }

  HANDLE GetData(UINT format) const {
    return ::GetClipboardData(format);
  }

  HANDLE SetData(UINT format, HANDLE mem) const {
    return ::SetClipboardData(format, mem);
  }

  std::wstring GetText() const {
    std::wstring str;

    if (::IsClipboardFormatAvailable(CF_UNICODETEXT)) {
      const auto handle = ::GetClipboardData(CF_UNICODETEXT);
      if (handle) {
        const auto text = reinterpret_cast<LPCWSTR>(::GlobalLock(handle));
        if (text) {
          str = text;
          ::GlobalUnlock(handle);
        }
      }
    }

    return str;
  }

  bool SetText(const std::wstring& str) const {
    const auto handle =
        ::GlobalAlloc(GMEM_MOVEABLE, (str.size() + 1) * sizeof(WCHAR));
    if (!handle)
      return false;

    const auto text = reinterpret_cast<LPWSTR>(::GlobalLock(handle));
    ::lstrcpyW(text, str.data());
    ::GlobalUnlock(handle);

    return ::SetClipboardData(CF_UNICODETEXT, handle);
  }
};

}  // namespace win
