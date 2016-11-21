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

#include "win_ctrl.h"

namespace win {

ProgressBar::ProgressBar(HWND hwnd) {
  SetWindowHandle(hwnd);
}

void ProgressBar::PreCreate(CREATESTRUCT &cs) {
  cs.dwExStyle = 0;
  cs.lpszClass = PROGRESS_CLASS;
  cs.style = WS_CHILD | WS_VISIBLE;
}

void ProgressBar::OnCreate(HWND hwnd, LPCREATESTRUCT create_struct) {
  Window::OnCreate(hwnd, create_struct);
}

////////////////////////////////////////////////////////////////////////////////

UINT ProgressBar::GetPosition() {
  return SendMessage(PBM_GETPOS, 0, 0);
}

void ProgressBar::SetMarquee(bool enabled) {
  if (enabled) {
    SetStyle(PBS_MARQUEE, 0);
  } else {
    SetStyle(0, PBS_MARQUEE);
  }

  SendMessage(PBM_SETMARQUEE, enabled, 0);
}

UINT ProgressBar::SetPosition(UINT position) {
  return SendMessage(PBM_SETPOS, position, 0);
}

DWORD ProgressBar::SetRange(UINT min, UINT max) {
  return SendMessage(PBM_SETRANGE32, min, max);
}

UINT ProgressBar::SetState(UINT state) {
  return SendMessage(PBM_SETSTATE, state, 0);
}

}  // namespace win