#pragma once

#include <chrono>
#include <filesystem>
#include <string_view>

#include <wx/panel.h>

class Settings;
class wxFilePickerCtrl;
class wxSpinCtrl;
class wxStaticText;
class wxWindow;

class SettingsPanel : public wxPanel
{
public:
  SettingsPanel(wxWindow *parent, Settings &initialSettings);

  Settings getSettingsInput() const;

  std::chrono::milliseconds getDelay() const;

  std::filesystem::path getPositionPath() const;

private:
  void createControls(Settings &initialSettings);

  wxStaticText *createLabel(std::string_view labelText);

  void setUpLayout();

  wxFilePickerCtrl *pathCtrl;
  wxSpinCtrl *delayCtrl;
};