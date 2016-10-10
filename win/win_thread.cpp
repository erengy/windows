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
#include "win_thread.h"

namespace win {

////////////////////////////////////////////////////////////////////////////////

Thread::Thread()
    : thread_(nullptr),
      thread_id_(0) {
}

Thread::~Thread() {
  CloseThreadHandle();
}

bool Thread::CloseThreadHandle() {
  BOOL value = TRUE;

  if (thread_) {
    value = ::CloseHandle(thread_);
    thread_ = nullptr;
  }

  return value != FALSE;
}

bool Thread::CreateThread(LPSECURITY_ATTRIBUTES thread_attributes,
                          SIZE_T stack_size,
                          DWORD creation_flags) {
  thread_ = ::CreateThread(thread_attributes, stack_size, ThreadProcStatic,
                           this, creation_flags, &thread_id_);

  return thread_ != nullptr;
}

HANDLE Thread::GetThreadHandle() const {
  return thread_;
}

DWORD Thread::GetThreadId() const {
  return thread_id_;
}

DWORD Thread::ThreadProc() {
  return 0;
}

DWORD WINAPI Thread::ThreadProcStatic(LPVOID lpParam) {
  Thread* thread = reinterpret_cast<Thread*>(lpParam);
  return thread->ThreadProc();
}

////////////////////////////////////////////////////////////////////////////////

CriticalSection::CriticalSection() {
  ::InitializeCriticalSectionAndSpinCount(&critical_section_, 0);
}

CriticalSection::~CriticalSection() {
  ::DeleteCriticalSection(&critical_section_);
}

void CriticalSection::Enter() {
  ::EnterCriticalSection(&critical_section_);
}

void CriticalSection::Leave() {
  ::LeaveCriticalSection(&critical_section_);
}

bool CriticalSection::TryEnter() {
  return ::TryEnterCriticalSection(&critical_section_) != FALSE;
}

void CriticalSection::Wait() {
  while (!TryEnter())
    ::Sleep(1);
}

////////////////////////////////////////////////////////////////////////////////

Event::Event()
    : event_(nullptr) {
}

Event::~Event() {
  if (event_)
    ::CloseHandle(event_);
}

HANDLE Event::Create(LPSECURITY_ATTRIBUTES event_attributes, BOOL manual_reset,
                     BOOL initial_state, LPCTSTR name) {
  event_ = ::CreateEvent(event_attributes, manual_reset, initial_state, name);

  return event_;
}

////////////////////////////////////////////////////////////////////////////////

Lock::Lock(CriticalSection& critical_section)
    : critical_section_(critical_section) {
  critical_section_.Enter();
}

Lock::~Lock() {
  critical_section_.Leave();
}

////////////////////////////////////////////////////////////////////////////////

Mutex::Mutex()
    : mutex_(nullptr) {
}

Mutex::~Mutex() {
  if (mutex_)
    ::CloseHandle(mutex_);
}

HANDLE Mutex::Create(LPSECURITY_ATTRIBUTES mutex_attributes,
                     BOOL initial_owner, LPCTSTR name) {
  mutex_ = ::CreateMutex(mutex_attributes, initial_owner, name);

  return mutex_;
}

HANDLE Mutex::Open(DWORD desired_access, BOOL inherit_handle, LPCTSTR name) {
  mutex_ = ::OpenMutex(desired_access, inherit_handle, name);

  return mutex_;
}

bool Mutex::Release() {
  BOOL value = TRUE;

  if (mutex_) {
    value = ::ReleaseMutex(mutex_);
    mutex_ = nullptr;
  }

  return value != FALSE;
}

}  // namespace win