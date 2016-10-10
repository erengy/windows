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

#ifndef TAIGA_WIN_MENU_H
#define TAIGA_WIN_MENU_H

#include "win_main.h"

namespace win {

enum MenuItemType {
  kMenuItemDefault = 0,
  kMenuItemSeparator,
  kMenuItemSubmenu
};

class MenuItem {
public:
  bool checked, def, enabled, new_column, radio, visible;
  std::wstring action, name, submenu;
  int type;
};

class Menu {
public:
  void CreateItem(const std::wstring& action = L"",
                  const std::wstring& name = L"",
                  const std::wstring& submenu = L"",
                  bool checked = false,
                  bool def = false,
                  bool enabled = true,
                  bool newcolumn = false,
                  bool radio = false);

  std::vector<MenuItem> items;
  std::wstring name;
  std::wstring type;
};

class MenuList {
public:
  void Create(const std::wstring& name, const std::wstring& type);
  HMENU CreateNewMenu(const std::wstring& name, std::vector<HMENU>& menu_handles);
  Menu* FindMenu(const std::wstring& name);
  std::wstring Show(HWND hwnd, int x, int y, const std::wstring& name);

  std::map<std::wstring, Menu> menus;
};

}  // namespace win

#endif  // TAIGA_WIN_MENU_H