#pragma once

#include <wx/frame.h>

class Settings;
class SettingsPanel;
class wxButton;
class wxCommandEvent;

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