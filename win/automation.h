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
#include <UIAutomation.h>

namespace win {

class UIAutomation {
public:
  UIAutomation();
  ~UIAutomation();

  bool Initialize();
  void Release();

  IUIAutomationElement* ElementFromHandle(HWND hwnd) const;

  IUIAutomationElementArray* FindAllControls(
      IUIAutomationElement* element, long control_type_id, bool read_only) const;
  IUIAutomationElement* FindFirstControl(
      IUIAutomationElement* element, long control_type_id, bool read_only) const;

  int GetElementArrayLength(IUIAutomationElementArray* element_array) const;
  IUIAutomationElement* GetElementFromArrayIndex(
      IUIAutomationElementArray* element_array, int index) const;

  std::wstring GetElementName(IUIAutomationElement* element) const;
  std::wstring GetElementValue(IUIAutomationElement* element) const;

private:
  IUIAutomationCondition* BuildControlCondition(
      IUIAutomationElement* element, long control_type_id, bool read_only) const;

  IUIAutomation* ui_automation_;
};

}  // namespace win