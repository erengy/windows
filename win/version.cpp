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

#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif

#include <windows.h>

#include "version.h"

namespace win {

Version GetVersion() {
  static bool checked = false;
  static Version version = kVersionPreXp;

  if (!checked) {
    OSVERSIONINFOEX version_info;
    version_info.dwOSVersionInfoSize = sizeof(version_info);
    GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&version_info));

    if (version_info.dwMajorVersion == 5) {
      switch (version_info.dwMinorVersion) {
        case 0:
          version = kVersionPreXp;
          break;
        case 1:
          version = kVersionXp;
          break;
        case 2:
        default:
          version = kVersionServer2003;
          break;
      }
    } else if (version_info.dwMajorVersion == 6) {
      if (version_info.wProductType != VER_NT_WORKSTATION) {
        version = kVersionServer2008;
      } else {
        switch (version_info.dwMinorVersion) {
          case 0:
            version = kVersionVista;
            break;
          case 1:
            version = kVersion7;
            break;
          case 2:
            version = kVersion8;
            break;
          case 3:
          default:
            version = kVersion8_1;
            break;
        }
      }
    } else if (version_info.dwMajorVersion == 10) {
      switch (version_info.dwMinorVersion) {
        case 0:
        default:
          version = kVersion10;
          break;
      }
    } else if (version_info.dwMajorVersion > 10) {
      version = kVersionUnknown;
    }

    checked = true;
  }

  return version;
}

}  // namespace win
