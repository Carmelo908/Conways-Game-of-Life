#include <atomic>
#include <string>

#include <wx/button.h>
#include <wx/frame.h>

#include "../position.hpp"
#include "../settings.hpp"
#include "positionpanel.hpp"

#pragma once

class GameFrame : private wxFrame
{
public:
  GameFrame(const Settings &settings, std::unique_ptr<Position> &&position);

private:
  void setUpLayout();

  void gameLoop();

  void onButtonClick(wxCommandEvent &);

  void onClose(wxCloseEvent &);

  std::unique_ptr<Position> position;
  std::chrono::milliseconds delay;
  std::atomic<bool> isGameRunning;

  wxButton *button;
  PositionPanel *drawingPanel;
};