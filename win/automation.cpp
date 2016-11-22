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

#include "automation.h"

namespace win {

UIAutomation::UIAutomation()
    : ui_automation_(nullptr) {
}

UIAutomation::~UIAutomation() {
  Release();
}

bool UIAutomation::Initialize() {
  return SUCCEEDED(CoCreateInstance(CLSID_CUIAutomation, nullptr,
                                    CLSCTX_INPROC_SERVER, IID_IUIAutomation,
                                    reinterpret_cast<void**>(&ui_automation_)));
}

void UIAutomation::Release() {
  if (ui_automation_) {
    ui_automation_->Release();
    ui_automation_ = nullptr;
  }
}

////////////////////////////////////////////////////////////////////////////////

IUIAutomationElement* UIAutomation::ElementFromHandle(HWND hwnd) const {
  IUIAutomationElement* element = nullptr;

  if (ui_automation_)
    ui_automation_->ElementFromHandle(static_cast<UIA_HWND>(hwnd), &element);

  return element;
}

IUIAutomationElementArray* UIAutomation::FindAllControls(
    IUIAutomationElement* element, long control_type_id, bool read_only) const {
  IUIAutomationElementArray* found = nullptr;

  if (element) {
    auto condition = BuildControlCondition(element, control_type_id, read_only);
    if (condition) {
      element->FindAll(TreeScope_Descendants, condition, &found);
      condition->Release();
    }
  }

  return found;
}

IUIAutomationElement* UIAutomation::FindFirstControl(
    IUIAutomationElement* element, long control_type_id, bool read_only) const {
  IUIAutomationElement* found = nullptr;

  if (element) {
    auto condition = BuildControlCondition(element, control_type_id, read_only);
    if (condition) {
      element->FindFirst(TreeScope_Descendants, condition, &found);
      condition->Release();
    }
  }

  return found;
}

////////////////////////////////////////////////////////////////////////////////

int UIAutomation::GetElementArrayLength(
    IUIAutomationElementArray* element_array) const {
  if (element_array) {
    int length = 0;
    if (SUCCEEDED(element_array->get_Length(&length)))
      return length;
  }

  return 0;
}

IUIAutomationElement* UIAutomation::GetElementFromArrayIndex(
    IUIAutomationElementArray* element_array, int index) const {
  IUIAutomationElement* element = nullptr;

  if (element_array)
    element_array->GetElement(index, &element);

  return element;
}

std::wstring UIAutomation::GetElementName(IUIAutomationElement* element) const {
  if (!element)
    return std::wstring();

  std::wstring str;

  BSTR bstr = nullptr;
  if (SUCCEEDED(element->get_CurrentName(&bstr)) && bstr) {
    str = bstr;
    SysFreeString(bstr);
  }

  return str;
}

std::wstring UIAutomation::GetElementValue(IUIAutomationElement* element) const {
  if (!element)
    return std::wstring();

  std::wstring str;

  IUIAutomationValuePattern* value_pattern = nullptr;
  if (SUCCEEDED(element->GetCurrentPatternAs(UIA_ValuePatternId,
                                             IID_PPV_ARGS(&value_pattern)))) {
    if (value_pattern) {
      BSTR bstr = nullptr;
      if (SUCCEEDED(value_pattern->get_CurrentValue(&bstr)) && bstr) {
        str = bstr;
        SysFreeString(bstr);
      }
      value_pattern->Release();
    }
  }

  return str;
}

////////////////////////////////////////////////////////////////////////////////

IUIAutomationCondition* UIAutomation::BuildControlCondition(
    IUIAutomationElement* element, long control_type_id, bool read_only) const {
  if (!ui_automation_)
    return nullptr;

  // Create control view condition
  IUIAutomationCondition* control_view_condition;
  ui_automation_->get_ControlViewCondition(&control_view_condition);

  // Create control type condition
  VARIANT property;
  property.vt = VT_I4;
  property.lVal = control_type_id;
  IUIAutomationCondition* type_condition = nullptr;
  ui_automation_->CreatePropertyCondition(UIA_ControlTypePropertyId,
                                          property, &type_condition);

  // Create read-only value condition
  property.vt = VT_BOOL;
  property.boolVal = read_only ? VARIANT_TRUE : VARIANT_FALSE;
  IUIAutomationCondition* readonly_condition = nullptr;
  ui_automation_->CreatePropertyCondition(UIA_ValueIsReadOnlyPropertyId,
                                          property, &readonly_condition);

  // Merge conditions
  IUIAutomationCondition* condition = nullptr;
  std::vector<IUIAutomationCondition*> conditions;
  if (control_view_condition)
    conditions.push_back(control_view_condition);
  if (type_condition)
    conditions.push_back(type_condition);
  if (readonly_condition)
    conditions.push_back(readonly_condition);
  if (!conditions.empty()) {
    ui_automation_->CreateAndConditionFromNativeArray(
        conditions.data(), static_cast<int>(conditions.size()), &condition);
  }

  // Clean up
  for (auto& condition : conditions) {
    if (condition)
      condition->Release();
  }

  return condition;
}

}  // namespace win