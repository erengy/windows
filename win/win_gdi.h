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

#include "win_main.h"

namespace win {

class Dc {
public:
  Dc();
  Dc(HDC hdc);
  virtual ~Dc();

  void operator=(const HDC hdc);

  void AttachDc(HDC hdc);
  HDC  DetachDc();
  HDC  Get() const;

  // Brush
  void   AttachBrush(HBRUSH brush);
  void   CreateSolidBrush(COLORREF color);
  HBRUSH DetachBrush();

  // Font
  void  AttachFont(HFONT font);
  HFONT DetachFont();
  void  EditFont(LPCWSTR face_name = nullptr, INT size = -1, BOOL bold = -1, BOOL italic = -1, BOOL underline = -1);

  // Painting
  BOOL FillRect(const RECT& rect, HBRUSH brush) const;
  void FillRect(const RECT& rect, COLORREF color) const;

  // Bitmap
  void    AttachBitmap(HBITMAP bitmap);
  BOOL    BitBlt(int x, int y, int width, int height, HDC dc_src, int x_src, int y_src, DWORD rop) const;
  HBITMAP DetachBitmap();
  int     SetStretchBltMode(int mode);
  BOOL    StretchBlt(int x, int y, int width, int height, HDC dc_src, int x_src, int y_src, int width_src, int height_src, DWORD rop) const;

  // Text
  int      DrawText(LPCWSTR text, int count, const RECT& rect, UINT format) const;
  COLORREF GetTextColor() const;
  COLORREF SetBkColor(COLORREF color) const;
  int      SetBkMode(int bk_mode) const;
  COLORREF SetTextColor(COLORREF color) const;

private:
  HDC     dc_;
  HBITMAP bitmap_old_;
  HBRUSH  brush_old_;
  HFONT   font_old_;
};

////////////////////////////////////////////////////////////////////////////////

class Brush {
public:
  Brush();
  Brush(HBRUSH brush);
  ~Brush();

  HBRUSH Get() const;
  void Set(HBRUSH brush);

  operator HBRUSH() const;

private:
  HBRUSH brush_;
};

////////////////////////////////////////////////////////////////////////////////

class Font {
public:
  Font();
  Font(HFONT font);
  ~Font();

  HFONT Get() const;
  void Set(HFONT font);

  operator HFONT() const;

private:
  HFONT font_;
};

////////////////////////////////////////////////////////////////////////////////

class Rect : public RECT {
public:
  Rect();
  Rect(int l, int t, int r, int b);
  Rect(const RECT& rect);
  Rect(LPCRECT rect);

  BOOL operator==(const RECT& rect) const;
  BOOL operator!=(const RECT& rect) const;
  void operator=(const RECT& rect);

  int Height() const;
  int Width() const;

  void Copy(const RECT& rect);
  BOOL Equal(const RECT& rect) const;
  BOOL Inflate(int dx, int dy);
  BOOL Intersect(const RECT& rect1, const RECT& rect2);
  BOOL IsEmpty() const;
  BOOL Offset(int dx, int dy);
  BOOL PtIn(POINT point) const;
  BOOL Set(int left, int top, int right, int bottom);
  BOOL SetEmpty();
  BOOL Subtract(const RECT& rect1, const RECT& rect2);
  BOOL Union(const RECT& rect1, const RECT& rect2);
};

}  // namespace win
