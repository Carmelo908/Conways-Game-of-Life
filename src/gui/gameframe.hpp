#include <atomic>
#include <string>

#include <wx/wx.h>

#include "../position.hpp"
#include "drawingpanel.hpp"

#pragma once

class GameFrame : public wxFrame
{
public:
  GameFrame();

private:
  static Position openPosition(std::string filePath);
 
  void gameLoop();

  void onButtonClick(wxCommandEvent&);

  Position position;
  std::atomic<bool> isGameRunning;

  wxButton *button;
  DrawingPanel *drawingPanel;
};