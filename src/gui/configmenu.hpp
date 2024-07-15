#include <string_view>

#include <wx/wx.h>

#include "fieldspanel.hpp"
#include "../settingsdata.hpp"

#pragma once

class ConfigMenu : public wxFrame
{
public:
  ConfigMenu();

private:
  void createButton();

  void setUpLayout();

  static std::unique_ptr<Position> openPosition(std::string_view filePath);

  void OnAcceptButton(wxCommandEvent &);
  
  wxButton *acceptButton;
  FieldsPanel *fieldsPanel;
};