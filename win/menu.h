/*
MIT License

Copyright (c) 2010-2021 Eren Okka

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

#include <map>
#include <string>
#include <vector>

#include <windows.h>

namespace win {

enum class MenuType {
  Menu,
  PopupMenu,
};

enum class MenuItemType {
  Default,
  Separator,
  Submenu,
};

struct MenuItem {
  UINT id = 0;
  bool checked = false;
  bool def = false;
  bool enabled = true;
  bool new_column = false;
  bool radio = false;
  bool visible = true;
  std::wstring action;
  std::wstring name;
  std::wstring submenu;
  MenuItemType type = MenuItemType::Default;
};

class Menu {
public:
  void CreateItem(const std::wstring& action = L"",
                  const std::wstring& name = L"",
                  const std::wstring& submenu = L"",
                  bool checked = false,
                  bool def = false,
                  bool enabled = true,
                  bool new_column = false,
                  bool radio = false);

  std::vector<MenuItem> items;
  std::wstring name;
  MenuType type = MenuType::PopupMenu;

private:
  std::map<std::wstring, UINT> actions_;
};

class MenuList {
public:
  void Create(const std::wstring& name, const MenuType type);
  HMENU CreateNewMenu(const std::wstring& name, std::vector<HMENU>& menu_handles);

  Menu* FindMenu(const std::wstring& name);
  MenuItem* FindMenuItem(const UINT id);

  std::wstring Show(HWND hwnd, int x, int y, const std::wstring& name);

  std::map<std::wstring, Menu> menus;
};

}  // namespace win
