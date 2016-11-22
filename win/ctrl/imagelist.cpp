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

ImageList::ImageList()
    : image_list_(nullptr) {
}

ImageList::~ImageList() {
  Destroy();
}

void ImageList::operator=(const HIMAGELIST image_list) {
  SetHandle(image_list);
}

BOOL ImageList::Create(int cx, int cy) {
  Destroy();

  image_list_ = ::ImageList_Create(cx, cy, ILC_COLOR32 | ILC_MASK, 0, 0);

  return image_list_ != nullptr;
}

VOID ImageList::Destroy() {
  if (image_list_) {
    ::ImageList_Destroy(image_list_);
    image_list_ = nullptr;
  }
}

////////////////////////////////////////////////////////////////////////////////

int ImageList::AddBitmap(HBITMAP bitmap, COLORREF mask) {
  if (mask != CLR_NONE) {
    return ::ImageList_AddMasked(image_list_, bitmap, mask);
  } else {
    return ::ImageList_Add(image_list_, bitmap, nullptr);
  }
}

BOOL ImageList::BeginDrag(int track, int hotspot_x, int hotspot_y) {
  return ::ImageList_BeginDrag(image_list_, track, hotspot_x, hotspot_y);
}

BOOL ImageList::DragEnter(HWND hwnd_lock, int x, int y) {
  return ::ImageList_DragEnter(hwnd_lock, x, y);
}

BOOL ImageList::DragLeave(HWND hwnd_lock) {
  return ::ImageList_DragLeave(hwnd_lock);
}

BOOL ImageList::DragMove(int x, int y) {
  return ::ImageList_DragMove(x, y);
}

BOOL ImageList::Draw(int index, HDC hdc, int x, int y) {
  return ::ImageList_Draw(image_list_, index, hdc, x, y, ILD_NORMAL);
}

void ImageList::Duplicate(HIMAGELIST image_list) {
  Destroy();

  image_list_ = ::ImageList_Duplicate(image_list);
}

VOID ImageList::EndDrag() {
  ::ImageList_EndDrag();
}

HIMAGELIST ImageList::GetHandle() {
  return image_list_;
}

HICON ImageList::GetIcon(int index) {
  return ::ImageList_GetIcon(image_list_, index, ILD_NORMAL);
}

BOOL ImageList::GetIconSize(int& cx, int& cy) {
  return ::ImageList_GetIconSize(image_list_, &cx, &cy);
}

BOOL ImageList::Remove(int index) {
  return ::ImageList_Remove(image_list_, index);
}

VOID ImageList::SetHandle(HIMAGELIST image_list) {
  Destroy();

  image_list_ = image_list;
}

}  // namespace win