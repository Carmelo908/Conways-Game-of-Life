#include "gameframe.hpp"

#include <chrono>
#include <memory>
#include <thread>
#include <utility>

#include <wx/sizer.h>

template<class Rep, class Period>
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

GameFrame::GameFrame(const Settings &settings, std::unique_ptr<Position> &&pos)
  : wxFrame(nullptr, wxID_ANY, "Conway's Game of Life", wxDefaultPosition),
    position{std::move(pos)},
    delay{settings.getDelay()},
    isGameRunning{false}
{
  Bind(wxEVT_CLOSE_WINDOW, &GameFrame::onClose, this);
  SetFont(GetFont().Scale(1.3));
  createControls();
  setUpLayout();
  Show(true);
  Maximize();
}

void GameFrame::createControls()
{
  startButton = new wxButton(this, wxID_ANY, "Start");
  positionPanel = new PositionPanel(this, *position);
  generationLabel = new wxStaticText(this, wxID_ANY, "");
  cellsQuantityLabel = new wxStaticText(this, wxID_ANY, "");
  startButton->Bind(wxEVT_BUTTON, &GameFrame::onButtonClick, this);
  updatePositionLabels();
}

void GameFrame::setUpLayout()
{
  auto mainSizer{new wxBoxSizer(wxVERTICAL)};
  auto controlSizer{new wxBoxSizer(wxHORIZONTAL)};
  controlSizer->Add(generationLabel, wxALIGN_CENTER_VERTICAL);
  controlSizer->AddSpacer(20);
  controlSizer->Add(startButton, wxALIGN_CENTER_VERTICAL);
  controlSizer->AddSpacer(20);
  controlSizer->Add(cellsQuantityLabel, wxALIGN_CENTER_VERTICAL);
  mainSizer->Add(positionPanel, 0, wxFIXED_MINSIZE | wxALIGN_CENTER_HORIZONTAL);
  mainSizer->AddSpacer(20);
  mainSizer->Add(controlSizer, 0, wxALIGN_CENTER_HORIZONTAL);
  SetSizerAndFit(mainSizer);
}

void GameFrame::gameLoop()
{
  while (isGameRunning)
  {
    DelayTimer t{delay};
    position->advanceGen();
    positionPanel->Refresh();
    updatePositionLabels();
    wxYield();
    t.wait();
  }
}

void GameFrame::onButtonClick(wxCommandEvent &)
{
  if (isGameRunning)
  {
    startButton->SetLabelText("Start");
    isGameRunning = false;
  } else
  {
    isGameRunning = true;
    startButton->SetLabelText("Stop");
    gameLoop();
  }
}

void GameFrame::updatePositionLabels()
{
  auto value = std::to_string(position->getGenCount());
  auto generationText = std::string("Generation: ") + value;
  generationLabel->SetLabelText(generationText);
  value = std::to_string(position->getCellsQuantity());
  auto cellsQuantityText = std::string("Cells amount: ") + value;
  cellsQuantityLabel->SetLabelText(cellsQuantityText);
}

void GameFrame::onClose(wxCloseEvent &)
{
  isGameRunning = false;
  Show(false);
  Destroy();
}