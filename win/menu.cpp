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

#include <string>
#include <vector>

#include "menu.h"

namespace win {

HMENU MenuList::CreateNewMenu(const std::wstring& name,
                              std::vector<HMENU>& menu_handles) {
  const auto menu = FindMenu(name);

  if (!menu)
    return nullptr;

  const HMENU handle = [&menu]() {
    switch (menu->type) {
      case MenuType::Menu:
        return ::CreateMenu();
      default:
      case MenuType::PopupMenu:
        return ::CreatePopupMenu();
    }
  }();

  if (!handle)
    return nullptr;

  menu_handles.push_back(handle);

  for (size_t i = 0; i < menu->items.size(); ++i) {
    const auto item = &menu->items[i];

    if (!item->visible)
      continue;

    const UINT flags =
        (item->checked ? MF_CHECKED : 0) |
        (item->def ? MF_DEFAULT : 0) |
        (item->enabled ? MF_ENABLED : MF_GRAYED) |
        (item->new_column ? MF_MENUBARBREAK : 0) |
        (item->radio ? MFT_RADIOCHECK : 0);

    switch (item->type) {
      case MenuItemType::Default: {
        ::AppendMenu(handle, MF_STRING | flags, item->id, item->name.c_str());
        if (item->def) {
          ::SetMenuDefaultItem(handle, item->id, FALSE);
        }
        break;
      }

      case MenuItemType::Separator: {
        ::AppendMenu(handle, MF_SEPARATOR, 0, nullptr);
        break;
      }

      case MenuItemType::Submenu: {
        const auto submenu = FindMenu(item->submenu.c_str());
        if (submenu && submenu != menu) {
          const HMENU submenu_handle =
              CreateNewMenu(submenu->name.c_str(), menu_handles);
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
  const auto menu_handle = CreateNewMenu(name, menu_handles);

  if (!menu_handle)
    return std::wstring();

  if (!x && !y) {
    POINT point;
    ::GetCursorPos(&point);
    x = point.x;
    y = point.y;
  }

  constexpr UINT flags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD;
  const BOOL item_id =
      ::TrackPopupMenuEx(menu_handle, flags, x, y, hwnd, nullptr);

  for (auto handle : menu_handles) {
    ::DestroyMenu(handle);
  }

  if (item_id > 0) {
    if (const auto menu_item = FindMenuItem(item_id)) {
      return menu_item->action;
    }
  }

  return std::wstring();
}

void MenuList::Create(const std::wstring& name, const MenuType type) {
  menus[name].name = name;
  menus[name].type = type;
}

Menu* MenuList::FindMenu(const std::wstring& name) {
  const auto it = menus.find(name);
  return it != menus.end() ? &it->second : nullptr;
}

MenuItem* MenuList::FindMenuItem(const UINT id) {
  for (auto& [name, menu] : menus) {
    for (auto& menu_item : menu.items) {
      if (menu_item.id == id) {
        return &menu_item;
      }
    }
  }
  return nullptr;
}

void Menu::CreateItem(const std::wstring& action, const std::wstring& name,
                      const std::wstring& submenu,
                      bool checked, bool def, bool enabled,
                      bool new_column, bool radio) {
  MenuItem item;

  item.id = [this, &action]() {
    const auto it = actions_.find(action);
    if (it != actions_.end()) {
      return it->second;
    } else {
      static UINT item_id = 0;
      actions_[action] = ++item_id;
      return item_id;
    }
  }();

  item.action = action;
  item.checked = checked;
  item.def = def;
  item.enabled = enabled;
  item.name = name;
  item.new_column = new_column;
  item.radio = radio;
  item.submenu = submenu;
  item.visible = true;

  if (!submenu.empty()) {
    item.type = MenuItemType::Submenu;
  } else if (name.empty()) {
    item.type = MenuItemType::Separator;
  } else {
    item.type = MenuItemType::Default;
  }

  items.push_back(item);
}

}  // namespace win
