#pragma once

#include <wx/frame.h>

#include "../settings.hpp"
#include "settingspanel.hpp"

class Position;
class PositionPanel;
class Settings;
class SettingsPanel;
class wxButton;
class wxFileDirPickerEvent;
class wxTimer;
class wxTimerEvent;
class wxStaticText;

/// @brief The window in which the Conway's Game of life is runned.
///
/// It contains all the controls, creates it's own layout and controls the game.
class GameFrame : private wxFrame
{
public:
  /// @brief GameFrame constructor.
  /// @param settings: settings object which is provided in the OnInit() of the.
  /// app
  GameFrame(std::unique_ptr<Settings> &&settings);

private:
  class Layout;

  void createComponents();

  void onGameTimer(wxTimerEvent &);

  void onStartButtonClick(wxCommandEvent &);

  void onSettingsChanged(SettingsUpdateEvent &e);

  void updatePositionLabels();

  void changePosition(std::unique_ptr<Position> &&position);

  void onClose(wxCloseEvent &);

  std::unique_ptr<Position> position;
  std::shared_ptr<Settings> settings;

  wxTimer *gameTimer;

  wxButton *startButton;
  PositionPanel *positionPanel;
  SettingsPanel *settingsPanel;
  wxStaticText *generationLabel;
  wxStaticText *cellsQuantityLabel;
};