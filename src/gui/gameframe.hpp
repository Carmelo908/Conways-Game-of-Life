#include <atomic>
#include <string>

#include <wx/wx.h>

#include "../position.hpp"
#include "../settingsdata.hpp"
#include "drawingpanel.hpp"

#pragma once

class GameFrame : private wxFrame
{
public:
  GameFrame(SettingsData &settings);

private:
  void gameLoop();

  std::chrono::milliseconds
  timeElapsed(std::chrono::steady_clock::time_point startTime) const;

  void onButtonClick(wxCommandEvent &);

  void onClose(wxCloseEvent &);

  std::unique_ptr<Position> position;
  std::chrono::milliseconds delay;
  std::atomic<bool> isGameRunning;

  wxButton *button;
  DrawingPanel *drawingPanel;
};