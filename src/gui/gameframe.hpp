#pragma once

#include <chrono>
#include <wx/frame.h>

#include "settingspanel.hpp"

class GameManager;
class Position;
class PositionPanel;
class Settings;
class SettingsPanel;
class SettingsUpdateEvent;
class wxButton;
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
  /// appSettingsUpdateEvent
  GameFrame(std::unique_ptr<Settings> &&settings);

private:
  class Layout;

  void createComponents();

  void onGameTimer(wxTimerEvent &);

  void onStartButtonClick(wxCommandEvent &);

  void onSettingsChanged(SettingsUpdateEvent &event);

  void updatePositionLabels(const Position &position);

  void changePosition(std::unique_ptr<Position> &&position);

  void onClose(wxCloseEvent &);

  std::unique_ptr<GameManager> gameManager;
  std::chrono::milliseconds delay;

  wxTimer *gameTimer;

  wxButton *startButton;
  PositionPanel *positionPanel;
  SettingsPanel *settingsPanel;
  wxStaticText *generationLabel;
  wxStaticText *cellsQuantityLabel;
};