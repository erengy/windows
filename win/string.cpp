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

#include <string>
#include <vector>

#include "string.h"

namespace win {

std::wstring StrToWstr(const std::string& str, UINT code_page) {
  if (!str.empty()) {
    int length = MultiByteToWideChar(code_page, 0, str.c_str(), -1, nullptr, 0);
    if (length > 0) {
      std::vector<wchar_t> output(length);
      MultiByteToWideChar(code_page, 0, str.c_str(), -1, &output[0], length);
      return &output[0];
    }
  }

  return std::wstring();
}

std::string WstrToStr(const std::wstring& str, UINT code_page) {
  if (!str.empty()) {
    int length = WideCharToMultiByte(code_page, 0, str.c_str(), -1, nullptr, 0,
                                     nullptr, nullptr);
    if (length > 0) {
      std::vector<char> output(length);
      WideCharToMultiByte(code_page, 0, str.c_str(), -1, &output[0], length,
                          nullptr, nullptr);
      return &output[0];
    }
  }

  return std::string();
}

////////////////////////////////////////////////////////////////////////////////

void ReadStringFromResource(LPCWSTR name, LPCWSTR type, std::wstring& output) {
  HRSRC hResInfo = FindResource(nullptr, name, type);
  HGLOBAL hResHandle = LoadResource(nullptr, hResInfo);
  DWORD dwSize = SizeofResource(nullptr, hResInfo);

  const char* lpData = static_cast<char*>(LockResource(hResHandle));
  std::string temp(lpData, dwSize);
  output = StrToWstr(temp);

  FreeResource(hResInfo);
}

}  // namespace win
