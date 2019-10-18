/*
MIT License

Copyright (c) 2010-2019 Eren Okka

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

#include "application.h"
#include "window_map.h"

namespace win {

Window* WindowMap::GetWindow(HWND hwnd) const {
  auto it = window_map_.find(hwnd);
  return it != window_map_.end() ? it->second : nullptr;
}

void WindowMap::Add(HWND hwnd, Window* window) {
  if (hwnd && !GetWindow(hwnd))
    window_map_.insert(std::make_pair(hwnd, window));
}

void WindowMap::Clear() {
  for (const auto& pair : window_map_) {
    const HWND hwnd = pair.first;
    if (::IsWindow(hwnd))
      ::DestroyWindow(hwnd);
  }

  window_map_.clear();
}

void WindowMap::Remove(HWND hwnd) {
  window_map_.erase(hwnd);
}

void WindowMap::Remove(Window* window) {
  for (auto it = window_map_.begin(); it != window_map_.end(); ++it) {
    if (window == it->second) {
      window_map_.erase(it);
      return;
    }
  }
}

WindowMap& WindowMap::Instance() {
  return App::Instance().window_map_;
}

}  // namespace win
