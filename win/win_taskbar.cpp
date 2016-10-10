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

#include "win_main.h"
#include "win_taskbar.h"

class win::Taskbar Taskbar;
class win::TaskbarList TaskbarList;

const DWORD WM_TASKBARCALLBACK = WM_APP + 0x15;
const DWORD WM_TASKBARCREATED = ::RegisterWindowMessage(L"TaskbarCreated");
const DWORD WM_TASKBARBUTTONCREATED = ::RegisterWindowMessage(L"TaskbarButtonCreated");

namespace win {

const UINT kAppSysTrayId = 74164;  // TAIGA ^_^

Taskbar::Taskbar()
    : hwnd_(nullptr) {
  Version version = GetVersion();
  if (version >= kVersionVista) {
    data_.cbSize = sizeof(NOTIFYICONDATA);
  } else if (version >= kVersionXp) {
    data_.cbSize = NOTIFYICONDATA_V3_SIZE;
  } else {
    data_.cbSize = NOTIFYICONDATA_V2_SIZE;
  }
}

Taskbar::~Taskbar() {
  Destroy();
}

BOOL Taskbar::Create(HWND hwnd, HICON icon, LPCWSTR tip) {
  Destroy();

  hwnd_ = hwnd;

  data_.hIcon = icon;
  data_.hWnd = hwnd;
  data_.szTip[0] = (WCHAR)'\0';
  data_.uCallbackMessage = WM_TASKBARCALLBACK;
  data_.uID = kAppSysTrayId;
  data_.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

  if (!icon)
    data_.hIcon = reinterpret_cast<HICON>(LoadImage(
        GetModuleHandle(nullptr), MAKEINTRESOURCE(101), IMAGE_ICON,
        GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
        LR_DEFAULTCOLOR));

  if (tip)
    wcscpy_s(data_.szTip, tip);

  return ::Shell_NotifyIcon(NIM_ADD, &data_);
}

BOOL Taskbar::Destroy() {
  if (!hwnd_)
    return FALSE;

  return ::Shell_NotifyIcon(NIM_DELETE, &data_);
}

BOOL Taskbar::Modify(LPCWSTR tip) {
  if (!hwnd_)
    return FALSE;

  data_.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  wcsncpy_s(data_.szTip, 128, tip, _TRUNCATE);

  return ::Shell_NotifyIcon(NIM_MODIFY, &data_);
}

BOOL Taskbar::Tip(LPCWSTR lpText, LPCWSTR lpTitle, int iIconIndex) {
  if (!hwnd_)
    return FALSE;

  data_.uFlags = NIF_INFO;
  data_.dwInfoFlags = iIconIndex;
  wcsncpy_s(data_.szInfo, 256, lpText, _TRUNCATE);
  wcsncpy_s(data_.szInfoTitle, 64, lpTitle, _TRUNCATE);

  return ::Shell_NotifyIcon(NIM_MODIFY, &data_);
}

////////////////////////////////////////////////////////////////////////////////

TaskbarList::TaskbarList()
    : hwnd_(nullptr),
    taskbar_list_(nullptr) {
}

TaskbarList::~TaskbarList() {
  Release();
}

void TaskbarList::Initialize(HWND hwnd) {
  Release();

  hwnd_ = hwnd;

  ::CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER,
                     __uuidof(ITaskbarList3), (void**)&taskbar_list_);
}

void TaskbarList::Release() {
  if (taskbar_list_) {
    taskbar_list_->Release();
    taskbar_list_ = nullptr;
    hwnd_ = nullptr;
  }
}

void TaskbarList::SetProgressState(TBPFLAG flag) {
  if (taskbar_list_)
    taskbar_list_->SetProgressState(hwnd_, flag);
}

void TaskbarList::SetProgressValue(ULONGLONG value, ULONGLONG total) {
  if (taskbar_list_)
    taskbar_list_->SetProgressValue(hwnd_, value, total);
}

}  // namespace win