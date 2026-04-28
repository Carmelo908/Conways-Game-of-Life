#pragma once

#include <atomic>
#include <chrono>
#include <string>

#include <wx/frame.h>

class Position;
class PositionPanel;
class Settings;
class wxButton;
class wxSpinCtrl;
class wxStaticText;

class GameFrame : private wxFrame
{
public:
  GameFrame(const Settings &settings, std::unique_ptr<Position> &&position);

private:
  void createControls();

  void setUpLayout();

  void gameLoop();

  void onButtonClick(wxCommandEvent &);

  void updatePositionLabels();

  void onClose(wxCloseEvent &);

  std::unique_ptr<Position> position;
  std::chrono::milliseconds delay;
  std::atomic_bool isGameRunning;

  wxButton *startButton;
  PositionPanel *positionPanel;
  wxStaticText *generationLabel;
  wxStaticText *cellsQuantityLabel;
};