#include <filesystem>

#include <wx/wx.h>

#include "../settingsdata.hpp"
#include "fieldspanel.hpp"

#pragma once

class ConfigMenu : public wxFrame
{
public:
  ConfigMenu(SettingsData &initialSettings);

private:
  void createButton();

  void setUpLayout();

  void OnAcceptButton(wxCommandEvent &);

  wxButton *acceptButton;
  FieldsPanel *fieldsPanel;
};