#include <chrono>
#include <memory>
#include <thread>
#include <utility>

#include <nlohmann/json.hpp>

#include "gameframe.hpp"

namespace chrono = std::chrono;

GameFrame::GameFrame(SettingsData &settings)
  : wxFrame(nullptr, wxID_ANY, "Conway's Game of Life", wxDefaultPosition),
    position{settings.releaseOpenedPosition()},
    delay{settings.delay},
    isGameRunning{false},
    button{new wxButton(this, wxID_ANY, "Start", wxDefaultPosition)},
    drawingPanel{new DrawingPanel(this, position.get())}
{
  Bind(wxEVT_CLOSE_WINDOW, &GameFrame::onClose, this);

  button->Bind(wxEVT_BUTTON, &GameFrame::onButtonClick, this);

  wxBoxSizer *boxSizer{new wxBoxSizer(wxVERTICAL)};
  boxSizer->Add(drawingPanel, 0, wxFIXED_MINSIZE | wxALIGN_CENTER_HORIZONTAL);
  boxSizer->Add(button, 0, wxALIGN_CENTER_HORIZONTAL);

  SetSizerAndFit(boxSizer);
  SetBackgroundColour(wxColour(255, 0, 0));
  Show(true);
  Maximize();
}

void GameFrame::gameLoop()
{
  while (isGameRunning)
  {
    auto startTime = chrono::steady_clock::now();

    position->advanceGen();
    drawingPanel->Refresh();

    std::this_thread::sleep_for(delay - timeElapsed(startTime));
  }
}

chrono::milliseconds
GameFrame::timeElapsed(chrono::steady_clock::time_point startPoint) const
{
  auto endTime = chrono::steady_clock::now();
  return chrono::duration_cast<chrono::milliseconds>(
      chrono::steady_clock::now() - startPoint);
}

void GameFrame::onButtonClick(wxCommandEvent &)
{
  if (isGameRunning)
  {
    isGameRunning = false;
    return;
  }
  isGameRunning = true;
  std::thread(&GameFrame::gameLoop, this).detach();
}

void GameFrame::onClose(wxCloseEvent &)
{
  isGameRunning = false;
  Close();
}