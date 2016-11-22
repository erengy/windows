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

Spin::Spin(HWND hwnd) {
  SetWindowHandle(hwnd);
}

bool Spin::GetPos32(int& value) {
  BOOL result;
  value = SendMessage(UDM_GETPOS32, 0, reinterpret_cast<LPARAM>(&result));
  return result == 0;
}

HWND Spin::SetBuddy(HWND hwnd) {
  return reinterpret_cast<HWND>(SendMessage(
      UDM_SETBUDDY, reinterpret_cast<WPARAM>(hwnd), 0));
}

int Spin::SetPos32(int position) {
  return SendMessage(UDM_SETPOS32, 0, position);
}

void Spin::SetRange32(int lower_limit, int upper_limit) {
  SendMessage(UDM_SETRANGE32, lower_limit, upper_limit);
}

}  // namespace win