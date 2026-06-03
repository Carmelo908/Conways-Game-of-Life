#pragma once

#include <chrono>
#include <filesystem>
#include <memory>
#include <string_view>

#include <wx/event.h>
#include <wx/panel.h>
#include <wx/slider.h>

#include "../settings.hpp"

class wxFilePickerCtrl;
class wxSpinCtrl;
class wxSlider;
class wxStaticText;

/// @brief represents a event that is going to be posted by SettingsPanel to its
/// father (GameFrame)whenever the user changes setting.
class SettingsUpdateEvent : public wxCommandEvent
{
public:
  SettingsUpdateEvent(std::shared_ptr<Settings> &&settings,
                      bool positionChanged);

  /// @return the settings created by the component posting the event.
  std::shared_ptr<Settings> getSettings();

  /// @return wether the user has opened a new Position. In this case, the
  /// processor of the event is expected to react on consequence.
  bool hasPositionChanged() const;

  /// @brief Provided only because wxWidgets event processing functionality
  /// requires it.
  /// @return a copy of the event object.
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

  /// @return a Settings instance created from the values of the input controls
  /// in the panel.
  Settings getSettings() const;

private:
  std::chrono::milliseconds getDelay() const;

  std::filesystem::path getPositionPath() const;

  int getZoom() const;

  void createControls(const Settings &initialSettings);

  wxStaticText *createLabel(std::string_view labelText);

  void setUpLayout();

  void onSettingsChanged(wxCommandEvent &event);

  wxFilePickerCtrl *pathCtrl;
  wxSpinCtrl *delayCtrl;
  wxSlider *zoomCtrl;
};