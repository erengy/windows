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

#include <string>
#include <vector>

#include "task_dialog.h"

namespace win {

std::vector<std::wstring> button_text;

LRESULT CALLBACK MsgBoxHookProc(int nCode, WPARAM wParam, LPARAM lParam);

TaskDialog::TaskDialog() {
  Initialize();
}

TaskDialog::TaskDialog(LPCWSTR title, LPWSTR icon) {
  Initialize();
  SetWindowTitle(title);
  SetMainIcon(icon);
}

HRESULT CALLBACK TaskDialog::Callback(HWND hwnd, UINT uNotification,
                                      WPARAM wParam, LPARAM lParam,
                                      LONG_PTR dwRefData) {
  switch (uNotification) {
    case TDN_DIALOG_CONSTRUCTED:
//    ::SetForegroundWindow(hwnd);
//    ::BringWindowToTop(hwnd);
      break;
    case TDN_HYPERLINK_CLICKED:
      ::ShellExecute(nullptr, nullptr, reinterpret_cast<LPCWSTR>(lParam),
                     nullptr, nullptr, SW_SHOWNORMAL);
      break;
    case TDN_VERIFICATION_CLICKED: {
      auto dlg = reinterpret_cast<TaskDialog*>(dwRefData);
      dlg->verification_checked_ = static_cast<BOOL>(wParam) == TRUE;
      break;
    }
  }

  return S_OK;
}

void TaskDialog::Initialize() {
  ::ZeroMemory(&config_, sizeof(TASKDIALOGCONFIG));

  config_.cbSize = sizeof(TASKDIALOGCONFIG);
  config_.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION |
                    TDF_ENABLE_HYPERLINKS |
                    TDF_POSITION_RELATIVE_TO_WINDOW |
                    TDF_SIZE_TO_CONTENT;
  config_.hInstance = ::GetModuleHandle(nullptr);
  config_.lpCallbackData = reinterpret_cast<LONG_PTR>(this);
  config_.pfCallback = Callback;

  selected_button_id_ = 0;
  verification_checked_ = false;
}

////////////////////////////////////////////////////////////////////////////////

void TaskDialog::AddButton(LPCWSTR text, int id) {
  buttons_.resize(buttons_.size() + 1);
  buttons_.back().pszButtonText = text;
  buttons_.back().nButtonID = id;
}

int TaskDialog::GetSelectedButtonID() const {
  return selected_button_id_;
}

bool TaskDialog::GetVerificationCheck() const {
  return verification_checked_;
}

void TaskDialog::SetCollapsedControlText(LPCWSTR text) {
  config_.pszCollapsedControlText = text;
}

void TaskDialog::SetContent(LPCWSTR text) {
  config_.pszContent = text;
}

void TaskDialog::SetExpandedControlText(LPCWSTR text) {
  config_.pszExpandedControlText = text;
}

void TaskDialog::SetExpandedInformation(LPCWSTR text) {
  config_.pszExpandedInformation = text;
}

void TaskDialog::SetFooter(LPCWSTR text) {
  config_.pszFooter = text;
}

void TaskDialog::SetFooterIcon(LPWSTR icon) {
  config_.dwFlags &= ~TDF_USE_HICON_FOOTER;
  config_.pszFooterIcon = icon;
}

void TaskDialog::SetMainIcon(LPWSTR icon) {
  config_.dwFlags &= ~TDF_USE_HICON_MAIN;
  config_.pszMainIcon = icon;
}

void TaskDialog::SetMainInstruction(LPCWSTR text) {
  config_.pszMainInstruction = text;
}

void TaskDialog::SetVerificationText(LPCWSTR text) {
  config_.pszVerificationText = text;
}

void TaskDialog::SetWindowTitle(LPCWSTR text) {
  config_.pszWindowTitle = text;
}

void TaskDialog::UseCommandLinks(bool use) {
  if (use) {
    config_.dwFlags |= TDF_USE_COMMAND_LINKS;
  } else {
    config_.dwFlags &= ~TDF_USE_COMMAND_LINKS;
  }
}

////////////////////////////////////////////////////////////////////////////////

HRESULT TaskDialog::Show(HWND parent) {
  config_.hwndParent = parent;
  if (!buttons_.empty()) {
    config_.pButtons = &buttons_[0];
    config_.cButtons = static_cast<UINT>(buttons_.size());
    if (buttons_.size() > 1)
      config_.dwFlags &= ~TDF_ALLOW_DIALOG_CANCELLATION;
  }

  // Show task dialog
  BOOL verification_flag_checked = TRUE;
  return ::TaskDialogIndirect(&config_, &selected_button_id_, nullptr,
                              &verification_flag_checked);
}

// MessageBox hook - used to change button text.
// Note that button width is fixed and does not change according to text length.
LRESULT CALLBACK MsgBoxHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
  auto cwpr = reinterpret_cast<LPCWPRETSTRUCT>(lParam);
  HWND hwnd = cwpr->hwnd;

  if (!(nCode < 0)) {
    WCHAR class_name[MAX_PATH];
    GetClassName(hwnd, class_name, MAX_PATH);

    if (!lstrcmpi(class_name, L"#32770")) {
      switch (cwpr->message) {
        case WM_INITDIALOG:
          #define SETBUTTONTEXT(x, y) \
            SendMessage(GetDlgItem(hwnd, x), WM_SETTEXT, 0, \
                        reinterpret_cast<LPARAM>(y.c_str()))
          switch (button_text.size()) {
            case 2:
              SETBUTTONTEXT(IDYES, button_text[0]);
              SETBUTTONTEXT(IDNO, button_text[1]);
              break;
            case 3:
              SETBUTTONTEXT(IDYES, button_text[0]);
              SETBUTTONTEXT(IDNO, button_text[1]);
              SETBUTTONTEXT(IDCANCEL, button_text[2]);
              break;
            default:
              SETBUTTONTEXT(IDCANCEL, button_text[0]);
          }
          #undef SETBUTTONTEXT
      }
    }
  }

  HWND parent = GetParent(hwnd);
  if (parent == nullptr)
    parent = GetDesktopWindow();

  return CallNextHookEx(reinterpret_cast<HHOOK>(GetProp(parent, L"MsgBoxHook")),
                        nCode, wParam, lParam);
}

}  // namespace win
