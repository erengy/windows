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

class Thread {
public:
  Thread();
  virtual ~Thread();

  virtual DWORD ThreadProc();

  bool CloseThreadHandle();
  bool CreateThread(
      LPSECURITY_ATTRIBUTES thread_attributes,
      SIZE_T stack_size,
      DWORD creation_flags);

  HANDLE GetThreadHandle() const;
  DWORD GetThreadId() const;

private:
  static DWORD WINAPI ThreadProcStatic(LPVOID thread);

  HANDLE thread_;
  DWORD thread_id_;
};

////////////////////////////////////////////////////////////////////////////////

class CriticalSection {
public:
  CriticalSection();
  virtual ~CriticalSection();

  void Enter();
  void Leave();
  bool TryEnter();
  void Wait();

private:
  CRITICAL_SECTION critical_section_;
};

class Event {
public:
  Event();
  virtual ~Event();

  HANDLE Create(
      LPSECURITY_ATTRIBUTES event_attributes,
      BOOL manual_reset,
      BOOL initial_state,
      LPCTSTR name);

private:
  HANDLE event_;
};

class Lock {
public:
  Lock(CriticalSection& critical_section);
  virtual ~Lock();

private:
  CriticalSection& critical_section_;
};

class Mutex {
public:
  Mutex();
  virtual ~Mutex();

  HANDLE Create(
      LPSECURITY_ATTRIBUTES mutex_attributes,
      BOOL initial_owner,
      LPCTSTR name);
  HANDLE Open(
      DWORD desired_access,
      BOOL inherit_handle,
      LPCTSTR name);
  bool Release();

private:
  HANDLE mutex_;
};

}  // namespace win
