#include <string_view>

#include <toml++/impl/table.hpp>
#include <wx/wx.h>

#include "../settingsdata.hpp"
#include "fieldspanel.hpp"

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