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

#ifndef TAIGA_WIN_DDE_H
#define TAIGA_WIN_DDE_H

#include "win_main.h"

namespace win {

// A helper class to use Dynamic Data Exchange (DDE) protocol

class DynamicDataExchange {
public:
  DynamicDataExchange();
  ~DynamicDataExchange();

  BOOL Initialize(DWORD afCmd = APPCLASS_STANDARD | APPCMD_CLIENTONLY,
                  bool unicode = false);
  void UnInitialize();

  BOOL Connect(const std::wstring& service, const std::wstring& topic);
  void Disconnect();

  BOOL ClientTransaction(const std::wstring& item,
                         const std::wstring& data,
                         std::wstring* output,
                         UINT wType);
  BOOL IsAvailable();
  BOOL NameService(const std::wstring& service, UINT afCmd = DNS_REGISTER);

  virtual BOOL OnConnect() { return TRUE; }
  virtual void OnPoke() {}
  virtual void OnRequest() {}

private:
  HSZ CreateStringHandle(const std::wstring& str);
  void FreeStringHandle(HSZ str);

  static FNCALLBACK DdeCallback;

  HCONV conversation_;
  DWORD instance_;
  bool is_unicode_;
};

}  // namespace win

#endif  // TAIGA_WIN_DDE_H