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

TreeView::TreeView(HWND hwnd) {
  SetWindowHandle(hwnd);
}

void TreeView::PreCreate(CREATESTRUCT &cs) {
  cs.dwExStyle = WS_EX_CLIENTEDGE;
  cs.lpszClass = WC_TREEVIEW;
  cs.style = WS_CHILD | WS_VISIBLE | WS_TABSTOP |
             TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
}

void TreeView::OnCreate(HWND hwnd, LPCREATESTRUCT create_struct) {
  TreeView_SetExtendedStyle(hwnd, TVS_EX_DOUBLEBUFFER, 0);
  Window::OnCreate(hwnd, create_struct);
}

////////////////////////////////////////////////////////////////////////////////

BOOL TreeView::DeleteAllItems() {
  return TreeView_DeleteAllItems(window_);
}

BOOL TreeView::DeleteItem(HTREEITEM item) {
  return TreeView_DeleteItem(window_, item);
}

BOOL TreeView::Expand(HTREEITEM item, bool expand) {
  return TreeView_Expand(window_, item, expand ? TVE_EXPAND : TVE_COLLAPSE);
}

UINT TreeView::GetCheckState(HTREEITEM item) {
  return TreeView_GetCheckState(window_, item);
}

UINT TreeView::GetCount() {
  return TreeView_GetCount(window_);
}

BOOL TreeView::GetItem(LPTVITEM item) {
  return TreeView_GetItem(window_, item);
}

LPARAM TreeView::GetItemData(HTREEITEM item) {
  TVITEM tvi = {0};
  tvi.mask = TVIF_PARAM;
  tvi.hItem = item;
  TreeView_GetItem(window_, &tvi);

  return tvi.lParam;
}

HTREEITEM TreeView::GetNextItem(HTREEITEM item, UINT flag) {
  return TreeView_GetNextItem(window_, item, flag);
}

HTREEITEM TreeView::GetSelection() {
  return TreeView_GetSelection(window_);
}

HTREEITEM TreeView::HitTest(LPTVHITTESTINFO hti, bool get_cursor_pos) {
  if (get_cursor_pos) {
    GetCursorPos(&hti->pt);
    ScreenToClient(window_, &hti->pt);
  }

  return TreeView_HitTest(window_, hti);
}

HTREEITEM TreeView::InsertItem(LPCWSTR text, int image, LPARAM param,
                               HTREEITEM parent, HTREEITEM insert_after) {
  TVITEM tvi = {0};
  tvi.mask = TVIF_TEXT | TVIF_PARAM;
  tvi.pszText = const_cast<LPWSTR>(text);
  tvi.lParam = param;

  if (image > -1) {
    tvi.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvi.iImage = image;
    tvi.iSelectedImage = image;
  }

  TVINSERTSTRUCT tvis = {0};
  tvis.item = tvi;
  tvis.hInsertAfter = insert_after;
  tvis.hParent = parent;

  return TreeView_InsertItem(window_, &tvis);
}

BOOL TreeView::SelectItem(HTREEITEM item) {
  return TreeView_Select(window_, item, TVGN_CARET);
}

UINT TreeView::SetCheckState(HTREEITEM item, BOOL check) {
  TVITEM tvi = {0};
  tvi.mask = TVIF_HANDLE | TVIF_STATE;
  tvi.hItem = item;
  tvi.stateMask = TVIS_STATEIMAGEMASK;
  tvi.state = INDEXTOSTATEIMAGEMASK(check + 1);

  return TreeView_SetItem(window_, &tvi);
}

HIMAGELIST TreeView::SetImageList(HIMAGELIST image_list, INT image) {
  return TreeView_SetImageList(window_, image_list, image);
}

BOOL TreeView::SetItem(HTREEITEM item, LPCWSTR text) {
  TVITEM tvi = {0};
  tvi.mask = TVIF_HANDLE;
  tvi.hItem = item;

  if (!TreeView_GetItem(window_, &tvi))
    return FALSE;

  tvi.mask |= TVIF_TEXT;
  tvi.pszText = const_cast<LPWSTR>(text);
  return TreeView_SetItem(window_, &tvi);
}

int TreeView::SetItemHeight(SHORT cyItem) {
  return TreeView_SetItemHeight(window_, cyItem);
}

}  // namespace win