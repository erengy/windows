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

#include "version.h"

namespace win {

// We can't expect that the compiler has VersionHelpers.h,
// so use our own functions to check the Windows version.
// An underscore added to avoid name collision.
inline bool _IsWindowsVersionOrGreater(WORD major, WORD minor) {
  OSVERSIONINFOEXW vi = { sizeof(vi),
                          major, minor, 0, 0, {0}, 0 };
  return ::VerifyVersionInfoW(&vi,
            VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR,
            ::VerSetConditionMask(::VerSetConditionMask(::VerSetConditionMask(0,
                                      VER_MAJORVERSION, VER_GREATER_EQUAL),
                                      VER_MINORVERSION, VER_GREATER_EQUAL),
                                      VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL));
}

inline bool _IsWindowsServer() {
  OSVERSIONINFOEXW vi = { sizeof(vi),
                          0, 0, 0, 0, {0}, 0, 0, 0, VER_NT_WORKSTATION };
  return !::VerifyVersionInfoW(&vi, VER_PRODUCT_TYPE,
                        ::VerSetConditionMask(0, VER_PRODUCT_TYPE, VER_EQUAL));
}

Version GetVersion() {
  static bool checked = false;
  static Version version = kVersionUnknown;

  if (checked)
    return version;

  if (_IsWindowsVersionOrGreater(10, 0))
    version = kVersion10;
  else if (_IsWindowsVersionOrGreater(6, 3))
    version = kVersion8_1;
  else if (_IsWindowsVersionOrGreater(6, 2))
    version = kVersion8;
  else if (_IsWindowsVersionOrGreater(6, 1))
    version = kVersion7;
  else if (_IsWindowsVersionOrGreater(6, 0))
    version = _IsWindowsServer() ? kVersionServer2008 : kVersionVista;
  else if (_IsWindowsVersionOrGreater(5, 1))
    version = _IsWindowsServer() ? kVersionServer2003 : kVersionXp;
  else if (_IsWindowsVersionOrGreater(5, 0))
    version = kVersion2000;

  checked = true;
  return version;
}

}  // namespace win
