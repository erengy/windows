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

#include "error.h"

namespace win {

ErrorMode::ErrorMode(unsigned int mode) {
  error_mode_ = ::SetErrorMode(mode);
  ::SetErrorMode(error_mode_ | mode);
};

ErrorMode::~ErrorMode() {
  ::SetErrorMode(error_mode_);
};

////////////////////////////////////////////////////////////////////////////////

std::wstring FormatError(unsigned long error, const std::wstring* source) {
  DWORD flags = FORMAT_MESSAGE_IGNORE_INSERTS;
  HMODULE module_handle = nullptr;

  const DWORD size = 101;
  WCHAR buffer[size];

  if (source) {
    flags |= FORMAT_MESSAGE_FROM_HMODULE;
    module_handle = LoadLibrary(source->c_str());
    if (!module_handle)
      return std::wstring();
  } else {
    flags |= FORMAT_MESSAGE_FROM_SYSTEM;
  }

  DWORD language_id = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);

  if (FormatMessage(flags, module_handle, error, language_id,
                    buffer, size, nullptr)) {
      if (module_handle)
        FreeLibrary(module_handle);
      return buffer;
  } else {
    if (module_handle)
      FreeLibrary(module_handle);
    wchar_t buffer[65];
    _ultow_s(error, buffer, 65, 10);
    return std::wstring(buffer);
  }
}

}  // namespace win
