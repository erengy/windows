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

#define TD_ICON_NONE         static_cast<PCWSTR>(0)
#define TD_ICON_INFORMATION  TD_INFORMATION_ICON
#define TD_ICON_WARNING      TD_WARNING_ICON
#define TD_ICON_ERROR        TD_ERROR_ICON
#define TD_ICON_SHIELD       TD_SHIELD_ICON
#define TD_ICON_SHIELD_GREEN MAKEINTRESOURCE(-8)
#define TD_ICON_SHIELD_RED   MAKEINTRESOURCE(-7)

#define TDF_SIZE_TO_CONTENT 0x1000000

namespace win {

class TaskDialog {
public:
  TaskDialog();
  TaskDialog(LPCWSTR title, LPWSTR icon);
  virtual ~TaskDialog() {}

  void AddButton(LPCWSTR text, int id);
  int GetSelectedButtonID() const;
  bool GetVerificationCheck() const;
  void SetCollapsedControlText(LPCWSTR text);
  void SetContent(LPCWSTR text);
  void SetExpandedControlText(LPCWSTR text);
  void SetExpandedInformation(LPCWSTR text);
  void SetFooter(LPCWSTR LPCWSTR);
  void SetFooterIcon(LPWSTR icon);
  void SetMainIcon(LPWSTR icon);
  void SetMainInstruction(LPCWSTR text);
  void SetVerificationText(LPCWSTR text);
  void SetWindowTitle(LPCWSTR text);
  HRESULT Show(HWND parent);
  void UseCommandLinks(bool use);

protected:
  static HRESULT CALLBACK Callback(
      HWND hwnd, UINT uNotification,
      WPARAM wParam, LPARAM lParam,
      LONG_PTR dwRefData);

  void Initialize();

  std::vector<TASKDIALOG_BUTTON> buttons_;
  TASKDIALOGCONFIG config_;
  int selected_button_id_;
  bool verification_checked_;
};

}  // namespace win
