#pragma once

#include <chrono>
#include <filesystem>
#include <memory>
#include <string_view>

#include <wx/event.h>
#include <wx/panel.h>

#include "../settings.hpp"

class wxFilePickerCtrl;
class wxSpinCtrl;
class wxStaticText;

class SettingsUpdateEvent : public wxCommandEvent
{
public:
  SettingsUpdateEvent(std::shared_ptr<Settings> &&settings,
                      bool positionChanged);

  std::shared_ptr<Settings> getSettings();

  bool hasPositionChanged() const;

  wxEvent *Clone() const override;

private:
  std::shared_ptr<Settings> settings;
  bool positionChanged;
};

wxDECLARE_EVENT(EVT_SETTINGS_UPDATED, SettingsUpdateEvent);

/// @brief Panel which contains controls to change the settings of the program,
/// open a position file and contains methods for accesing those fields
class SettingsPanel : public wxPanel
{
public:
  /// @brief Creates the object, meant to be used inside GameFrame construction.
  /// @param parent: the parent window (GameFrame)
  /// @param initialSettings: settings openned on the program beggining.
  SettingsPanel(wxWindow *parent, const Settings &initialSettings);

  Settings getSettings() const;

private:
  std::chrono::milliseconds getDelay() const;

  std::filesystem::path getPositionPath() const;

  void createControls(const Settings &initialSettings);

  wxStaticText *createLabel(std::string_view labelText);

  void setUpLayout();

  void onSettingsChanged(wxCommandEvent &event);

  wxFilePickerCtrl *pathCtrl;
  wxSpinCtrl *delayCtrl;
};