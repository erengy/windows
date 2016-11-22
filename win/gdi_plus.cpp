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

#pragma comment(lib, "gdiplus.lib")

#include <memory>

#include <windows.h>
#include <gdiplus.h>

#include "gdi_plus.h"

namespace win {

GdiPlus::GdiPlus()
    : token_(0) {
  Gdiplus::GdiplusStartupInput input;
  Gdiplus::GdiplusStartup(&token_, &input, nullptr);
}

GdiPlus::~GdiPlus() {
  Gdiplus::GdiplusShutdown(token_);
  token_ = 0;
}

void GdiPlus::DrawRectangle(const HDC hdc, const RECT& rect,
                            Gdiplus::ARGB color) {
  const Gdiplus::SolidBrush brush = Gdiplus::Color(color);
  Gdiplus::Graphics graphics(hdc);
  graphics.FillRectangle(&brush, rect.left, rect.top,
                         rect.right - rect.left, rect.bottom - rect.top);
}

HICON GdiPlus::LoadIcon(const std::wstring& file) {
  HICON icon_handle = nullptr;

  std::unique_ptr<Gdiplus::Bitmap> bitmap(
      Gdiplus::Bitmap::FromFile(file.c_str()));

  if (bitmap)
    bitmap->GetHICON(&icon_handle);

  return icon_handle;
}

HBITMAP GdiPlus::LoadImage(const std::wstring& file, UINT width, UINT height) {
  HBITMAP bitmap_handle = nullptr;

  std::unique_ptr<Gdiplus::Bitmap> bitmap(
      Gdiplus::Bitmap::FromFile(file.c_str()));

  if (bitmap) {
    const Gdiplus::Color color(Gdiplus::Color::AlphaMask);

    if ((width > 0 && width != bitmap->GetWidth()) ||
        (height > 0 && height != bitmap->GetHeight())) {
      std::unique_ptr<Gdiplus::Bitmap> resized_bitmap(
          new Gdiplus::Bitmap(width, height));

      Gdiplus::Graphics graphics(resized_bitmap.get());
      graphics.ScaleTransform(
          width / static_cast<Gdiplus::REAL>(bitmap->GetWidth()),
          height / static_cast<Gdiplus::REAL>(bitmap->GetHeight()));
      graphics.DrawImage(bitmap.get(), 0, 0);

      resized_bitmap->GetHBITMAP(color, &bitmap_handle);

    } else {
      bitmap->GetHBITMAP(color, &bitmap_handle);
    }
  }

  return bitmap_handle;
}

}  // namespace win