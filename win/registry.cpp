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

#include <string>
#include <vector>

#include "registry.h"

namespace win {

Registry::Registry()
    : key_(nullptr) {
}

Registry::~Registry() {
  CloseKey();
}

LSTATUS Registry::CloseKey() {
  if (key_) {
    LSTATUS status = ::RegCloseKey(key_);
    key_ = nullptr;
    return status;
  } else {
    return ERROR_SUCCESS;
  }
}

LSTATUS Registry::CreateKey(HKEY key,
                            const std::wstring& subkey,
                            LPWSTR class_type,
                            DWORD options,
                            REGSAM sam_desired,
                            LPSECURITY_ATTRIBUTES security_attributes,
                            LPDWORD disposition) {
  CloseKey();

  return ::RegCreateKeyEx(
      key, subkey.c_str(), 0, class_type, options, sam_desired,
      security_attributes, &key_, disposition);
}

LONG Registry::DeleteKey(const std::wstring& subkey) {
  return DeleteSubkeys(key_, subkey);
}

LSTATUS Registry::DeleteSubkeys(HKEY root_key, const std::wstring& subkey) {
  LSTATUS status = ::RegDeleteKey(root_key, subkey.c_str());

  if (status == ERROR_SUCCESS)
    return status;

  HKEY key;
  status = ::RegOpenKeyEx(root_key, subkey.c_str(), 0, KEY_READ, &key);

  if (status != ERROR_SUCCESS)
    return status;

  DWORD subkey_count = 0;
  std::vector<std::wstring> keys;
  WCHAR name[MAX_PATH] = {L'\0'};

  if (::RegQueryInfoKey(key, nullptr, nullptr, nullptr, &subkey_count,
                        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                        nullptr) == ERROR_SUCCESS) {
    for (DWORD i = 0; i < subkey_count; i++) {
      DWORD name_length = MAX_PATH;
      if (::RegEnumKeyEx(key, i, name, &name_length, nullptr, nullptr, nullptr,
                         nullptr) == ERROR_SUCCESS) {
        keys.push_back(name);
      }
    }
  }

  for (auto it = keys.begin(); it != keys.end(); ++it) {
    DeleteSubkeys(key, it->c_str());
  }

  ::RegCloseKey(key);

  return ::RegDeleteKey(root_key, subkey.c_str());
}

LSTATUS Registry::DeleteValue(const std::wstring& value_name) {
  return ::RegDeleteValue(key_, value_name.c_str());
}

void Registry::EnumKeys(std::vector<std::wstring>& output) {
  // sam_desired must be: KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS

  WCHAR name[MAX_PATH] = {0};
  DWORD subkey_count = 0;

  if (::RegQueryInfoKey(key_, nullptr, nullptr, nullptr, &subkey_count,
                        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                        nullptr) == ERROR_SUCCESS) {
    for (DWORD i = 0; i < subkey_count; i++) {
      DWORD name_length = MAX_PATH;
      if (::RegEnumKeyEx(key_, i, name, &name_length, nullptr, nullptr, nullptr,
                         nullptr) == ERROR_SUCCESS) {
        output.push_back(name);
      }
    }
  }
}

LSTATUS Registry::OpenKey(HKEY key, const std::wstring& subkey, DWORD options,
                          REGSAM sam_desired) {
  CloseKey();

  return ::RegOpenKeyEx(key, subkey.c_str(), options, sam_desired, &key_);
}

LSTATUS Registry::QueryValue(const std::wstring& value_name, LPDWORD type,
                             LPBYTE data, LPDWORD data_size) {
  return ::RegQueryValueEx(key_, value_name.c_str(), nullptr, type,
                           data, data_size);
}

LSTATUS Registry::SetValue(const std::wstring& value_name, DWORD type,
                           CONST BYTE* data, DWORD data_size) {
  return ::RegSetValueEx(key_, value_name.c_str(), 0, type,
                         data, data_size);
}

std::wstring Registry::QueryValue(const std::wstring& value_name) {
  DWORD type = 0;
  WCHAR buffer[MAX_PATH];
  DWORD buffer_size = sizeof(buffer);

  if (QueryValue(value_name.c_str(), &type, reinterpret_cast<LPBYTE>(&buffer),
                 &buffer_size) == ERROR_SUCCESS)
    if (type == REG_SZ)
      return buffer;

  return std::wstring();
}

void Registry::SetValue(const std::wstring& value_name,
                        const std::wstring& value) {
  SetValue(value_name.c_str(), REG_SZ, (LPBYTE)value.c_str(),
           static_cast<DWORD>(value.size() * sizeof(WCHAR)));
}

}  // namespace win
