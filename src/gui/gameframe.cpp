#include "gameframe.hpp"

#include <chrono>
#include <memory>
#include <thread>
#include <utility>

#include <wx/sizer.h>
#include <nlohmann/json.hpp>

template <class Rep, class Period>
class DelayTimer
{
public:
  DelayTimer(std::chrono::duration<Rep, Period> delay)
    : elapsed{std::chrono::steady_clock::now() + delay}
  {}

  void wait() { std::this_thread::sleep_until(elapsed); }

private:
  std::chrono::steady_clock::time_point elapsed;
};

GameFrame::GameFrame(const SettingsData &settings,
                     std::unique_ptr<Position> &&position)
  : wxFrame(nullptr, wxID_ANY, "Conway's Game of Life", wxDefaultPosition),
    position{std::move(position)},
    delay{settings.delay},
    isGameRunning{false},
    button{new wxButton(this, wxID_ANY, "Start", wxDefaultPosition)},
    drawingPanel{new DrawingPanel(this, this->position.get())}
{
  Bind(wxEVT_CLOSE_WINDOW, &GameFrame::onClose, this);
  button->Bind(wxEVT_BUTTON, &GameFrame::onButtonClick, this);
  setUpLayout();
  SetBackgroundColour(wxColour(255, 0, 0));
  Show(true);
  Maximize();
}

void GameFrame::setUpLayout()
{
  wxBoxSizer *boxSizer{new wxBoxSizer(wxVERTICAL)};
  boxSizer->Add(drawingPanel, 0, wxFIXED_MINSIZE | wxALIGN_CENTER_HORIZONTAL);
  boxSizer->Add(button, 0, wxALIGN_CENTER_HORIZONTAL);
  SetSizerAndFit(boxSizer);
}

void GameFrame::gameLoop()
{
  while (isGameRunning)
  {
    DelayTimer t{delay};
    position->advanceGen();
    drawingPanel->Refresh();
    t.wait();
  }
}

void GameFrame::onButtonClick(wxCommandEvent &)
{
  if (isGameRunning)
  {
    isGameRunning = false;
    return;
  } else
  {
    isGameRunning = true;
    std::thread(&GameFrame::gameLoop, this).detach();
  }
}

void GameFrame::onClose(wxCloseEvent &)
{
  isGameRunning = false;
  Show(false);
  Destroy();
}