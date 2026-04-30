#pragma once

#include <atomic>
#include <chrono>
#include <string>

#include <wx/frame.h>

#include "../settings.hpp"

class Position;
class PositionPanel;
class Settings;
class SettingsPanel;
class wxButton;
class wxStaticText;
class wxFileDirPickerEvent;

class GameFrame : private wxFrame
{
public:
  GameFrame(Settings &settings);

private:
  void createComponents();

  wxSizer *createControlSizer() const;

  void setUpLayout();

  void gameLoop();

  void onStartButtonClick(wxCommandEvent &);

  void onPositionChanged(wxFileDirPickerEvent &event);

  void updatePositionLabels();

  void onClose(wxCloseEvent &);

  std::unique_ptr<Position> position;
  std::atomic_bool isGameRunning;
  Settings settings;

  wxButton *startButton;
  PositionPanel *positionPanel;
  SettingsPanel *settingsPanel;
  wxStaticText *generationLabel;
  wxStaticText *cellsQuantityLabel;
};