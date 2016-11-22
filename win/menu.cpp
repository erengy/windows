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

#include "menu.h"

namespace win {

HMENU MenuList::CreateNewMenu(const std::wstring& name,
                              std::vector<HMENU>& menu_handles) {
  auto menu = FindMenu(name);

  if (!menu)
    return nullptr;

  HMENU handle = nullptr;
  if (menu->type == L"menubar") {
    handle = ::CreateMenu();
  } else {
    handle = ::CreatePopupMenu();
  }

  if (!handle)
    return nullptr;

  menu_handles.push_back(handle);

  for (auto item = menu->items.begin(); item != menu->items.end(); ++item) {
    if (!item->visible)
      continue;
    const UINT flags =
        (item->checked ? MF_CHECKED : 0) |
        (item->def ? MF_DEFAULT : 0) |
        (item->enabled ? MF_ENABLED : MF_GRAYED) |
        (item->new_column ? MF_MENUBARBREAK : 0) |
        (item->radio ? MFT_RADIOCHECK : 0);
    switch (item->type) {
      case kMenuItemDefault: {
        UINT_PTR id_new_item = reinterpret_cast<UINT_PTR>(&item->action);
        ::AppendMenu(handle, MF_STRING | flags, id_new_item,
                     item->name.c_str());
        if (item->def)
          ::SetMenuDefaultItem(handle, id_new_item, FALSE);
        break;
      }
      case kMenuItemSeparator: {
        ::AppendMenu(handle, MF_SEPARATOR, 0, nullptr);
        break;
      }
      case kMenuItemSubmenu: {
        auto submenu = FindMenu(item->submenu.c_str());
        if (submenu && submenu != menu) {
          HMENU submenu_handle = CreateNewMenu(submenu->name.c_str(),
                                               menu_handles);
          ::AppendMenu(handle, MF_POPUP | flags,
                       reinterpret_cast<UINT_PTR>(submenu_handle),
                       item->name.c_str());
        }
        break;
      }
    }
  }

  return handle;
}

std::wstring MenuList::Show(HWND hwnd, int x, int y, const std::wstring& name) {
  std::vector<HMENU> menu_handles;
  CreateNewMenu(name, menu_handles);

  if (menu_handles.empty())
    return std::wstring();

  if (!x && !y) {
    POINT point;
    ::GetCursorPos(&point);
    x = point.x;
    y = point.y;
  }

  UINT flags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD;
  UINT_PTR index = ::TrackPopupMenuEx(menu_handles.front(),
                                      flags, x, y, hwnd, nullptr);

  for (auto it = menu_handles.begin(); it != menu_handles.end(); ++it)
    ::DestroyMenu(*it);

  if (index > 0) {
    auto str = reinterpret_cast<std::wstring*>(index);
    return *str;
  } else {
    return std::wstring();
  }
}

void MenuList::Create(const std::wstring& name, const std::wstring& type) {
  menus[name].name = name;
  menus[name].type = type;
}

Menu* MenuList::FindMenu(const std::wstring& name) {
  auto it = menus.find(name);
  if (it != menus.end())
    return &it->second;

  return nullptr;
}

void Menu::CreateItem(const std::wstring& action, const std::wstring& name,
                      const std::wstring& submenu,
                      bool checked, bool def, bool enabled,
                      bool newcolumn, bool radio) {
  MenuItem item;

  item.action = action;
  item.checked = checked;
  item.def = def;
  item.enabled = enabled;
  item.name = name;
  item.new_column = newcolumn;
  item.radio = radio;
  item.submenu = submenu;
  item.visible = true;

  if (!submenu.empty()) {
    item.type = kMenuItemSubmenu;
  } else if (name.empty()) {
    item.type = kMenuItemSeparator;
  } else {
    item.type = kMenuItemDefault;
  }

  items.push_back(item);
}

}  // namespace win