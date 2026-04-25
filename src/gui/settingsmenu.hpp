#include <filesystem>

#include <wx/button.h>

#include "../settings.hpp"
#include "settingspanel.hpp"

#pragma once

class SettingsMenu : public wxFrame
{
public:
  SettingsMenu(Settings &initialSettings);

private:
  void createButton();

  void setUpLayout();

  void OnAcceptButton(wxCommandEvent &);

  wxButton *acceptButton;
  SettingsPanel *fieldsPanel;
};