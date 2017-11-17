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

#include <windows.h>

#include "window.h"

namespace win {

class Dialog : public Window {
public:
  Dialog();
  virtual ~Dialog();

  virtual INT_PTR Create(UINT resource_id, HWND parent = nullptr, bool modal = true);
  virtual void EndDialog(INT_PTR result);
  virtual void SetSizeMax(LONG cx, LONG cy);
  virtual void SetSizeMin(LONG cx, LONG cy);
  virtual void SetSnapGap(int snap_gap);

  virtual BOOL AddComboString(int id_combo, LPCWSTR text);
  virtual BOOL CheckDlgButton(int id_button, UINT check);
  virtual BOOL CheckRadioButton(int id_first_button, int id_last_button, int id_check_button);
  virtual BOOL EnableDlgItem(int id_item, BOOL enable);
  virtual INT  GetCheckedRadioButton(int id_first_button, int id_last_button);
  virtual INT  GetComboSelection(int id_item);
  virtual HWND GetDlgItem(int id_item);
  virtual UINT GetDlgItemInt(int id_item);
  virtual void GetDlgItemText(int id_item, LPWSTR output, int max_length = MAX_PATH);
  virtual void GetDlgItemText(int id_item, std::wstring& output);
  virtual std::wstring GetDlgItemText(int id_item);
  virtual BOOL HideDlgItem(int id_item);
  virtual BOOL IsDlgButtonChecked(int id_button);
  virtual bool IsModal();
  virtual BOOL SendDlgItemMessage(int id_item, UINT uMsg, WPARAM wParam, LPARAM lParam);
  virtual BOOL SetComboSelection(int id_item, int index);
  virtual BOOL SetDlgItemText(int id_item, LPCWSTR text);
  virtual BOOL ShowDlgItem(int id_item, int cmd_show = SW_SHOWNORMAL);

protected:
  virtual void OnCancel();
  virtual BOOL OnClose();
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  virtual void RegisterDlgClass(LPCWSTR class_name);

  virtual INT_PTR DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  virtual INT_PTR DialogProcDefault(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
  using Window::Create; // Hide Create method from the base class

  static INT_PTR CALLBACK DialogProcStatic(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  void SetMinMaxInfo(LPMINMAXINFO mmi);
  void SnapToEdges(LPWINDOWPOS window_pos);

  bool  modal_;
  int   snap_gap_;
  SIZE  size_last_, size_max_, size_min_;
  POINT pos_last_;
};

}  // namespace win
