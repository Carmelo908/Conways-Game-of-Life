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

/// @brief Panel which contains controls to change the settings of the program,
/// open a position file and contains methods for accesing those fields
class SettingsPanel : public wxPanel
{
public:
  /// @brief Creates the object, meant to be used inside GameFrame construction.
  /// @param parent: the parent window (GameFrame)
  /// @param initialSettings: settings openned on the program beggining.
  SettingsPanel(wxWindow *parent, Settings &initialSettings);

  /// @brief Converts current fileds on itself to a Settings object and returns
  /// it
  Settings getSettingsInput() const;

  /// @brief Converts the value in the delay control to a
  /// std::chrono::milliseconds value.
  /// @return value inside control range [0, 1000]
  std::chrono::milliseconds getDelay() const;

  /// @brief Converts the value in the position control to a
  /// std::filesystem::path value
  /// @return path object which can be empty, contain a path to a missing file
  /// or contain a path to a file with invalid data.
  std::filesystem::path getPositionPath() const;

private:
  void createControls(Settings &initialSettings);

  wxStaticText *createLabel(std::string_view labelText);

  void setUpLayout();

  wxFilePickerCtrl *pathCtrl;
  wxSpinCtrl *delayCtrl;
};