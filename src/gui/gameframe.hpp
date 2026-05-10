#pragma once

#include <wx/frame.h>

#include "../settings.hpp"

class Position;
class PositionPanel;
class Settings;
class SettingsPanel;
class wxButton;
class wxStaticText;
class wxFileDirPickerEvent;

/// @brief The window in which the Conway's Game of life is runned.
///
/// It contains all the controls, creates it's own layout and controls the game.
class GameFrame : private wxFrame
{
public:
  /// @brief GameFrame constructor.
  /// @param settings: settings object which is provided in the OnInit() of the.
  /// app
  GameFrame(Settings &settings);

private:
  class Layout;

  void createComponents();

  void gameLoop();

  void onStartButtonClick(wxCommandEvent &);

  void onPositionOpened(wxFileDirPickerEvent &event);

  void updatePositionLabels();

  void onClose(wxCloseEvent &);

  std::unique_ptr<Position> position;
  bool isGameRunning;
  Settings settings;

  wxButton *startButton;
  PositionPanel *positionPanel;
  SettingsPanel *settingsPanel;
  wxStaticText *generationLabel;
  wxStaticText *cellsQuantityLabel;
};