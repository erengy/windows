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

#include <string>
#include <vector>

#include <windows.h>

namespace win {

class Registry {
public:
  Registry();
  ~Registry();

  LSTATUS CloseKey();
  LSTATUS CreateKey(
      HKEY key,
      const std::wstring& subkey,
      LPWSTR class_type = nullptr,
      DWORD options = REG_OPTION_NON_VOLATILE,
      REGSAM sam_desired = KEY_SET_VALUE,
      LPSECURITY_ATTRIBUTES security_attributes = nullptr,
      LPDWORD disposition = nullptr);
  LONG DeleteKey(const std::wstring& subkey);
  LSTATUS DeleteValue(const std::wstring& value_name);
  void EnumKeys(std::vector<std::wstring>& output);
  LSTATUS OpenKey(
      HKEY key,
      const std::wstring& subkey,
      DWORD options = 0,
      REGSAM sam_desired = KEY_QUERY_VALUE);
  LSTATUS QueryValue(
      const std::wstring& value_name,
      LPDWORD type,
      LPBYTE data,
      LPDWORD data_size);
  std::wstring QueryValue(const std::wstring& value_name);
  LSTATUS SetValue(
      const std::wstring& value_name,
      DWORD type,
      CONST BYTE* data,
      DWORD data_size);
  void SetValue(
      const std::wstring& value_name,
      const std::wstring& value);

private:
  LSTATUS DeleteSubkeys(HKEY root_key, const std::wstring& subkey);

  HKEY key_;
};

}  // namespace win
