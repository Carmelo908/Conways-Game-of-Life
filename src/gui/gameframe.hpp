#include <atomic>
#include <string>

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>

#include "../position.hpp"
#include "../settings.hpp"
#include "positionpanel.hpp"

#pragma once

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
  wxSpinCtrl *delayControl;
};