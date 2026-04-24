#include "gameframe.hpp"

#include <chrono>
#include <memory>
#include <thread>
#include <utility>

#include <wx/sizer.h>

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

GameFrame::GameFrame(const Settings &settings, std::unique_ptr<Position> &&pos)
  : wxFrame(nullptr, wxID_ANY, "Conway's Game of Life", wxDefaultPosition),
    position{std::move(pos)},
    delay{settings.delay},
    isGameRunning{false},
    startButton{new wxButton(this, wxID_ANY, "Start")},
    drawingPanel{new PositionPanel(this, this->position.get())},
    generationLabel{new wxStaticText(this, wxID_ANY, "")},
    cellAmountLabel{new wxStaticText(this, wxID_ANY, "")}
{
  Bind(wxEVT_CLOSE_WINDOW, &GameFrame::onClose, this);
  startButton->Bind(wxEVT_BUTTON, &GameFrame::onButtonClick, this);
  startButton->SetFont(startButton->GetFont().Scale(1.2));
  generationLabel->SetLabel("Alive cells: " +
                            std::to_string(position->getCellsQuantity()));
  cellAmountLabel->SetLabel("Generation: " +
                            std::to_string(position->getGenCount()));
  setUpLayout();
  Show(true);
  Maximize();
}

void GameFrame::setUpLayout()
{
  auto verticalSizer{new wxBoxSizer(wxVERTICAL)};
  auto controlSizer{new wxBoxSizer(wxHORIZONTAL)};
  controlSizer->Add(generationLabel, wxALIGN_CENTER_VERTICAL);
  controlSizer->AddSpacer(30);
  controlSizer->Add(startButton, wxALIGN_CENTER_VERTICAL);
  controlSizer->AddSpacer(30);
  controlSizer->Add(cellAmountLabel, wxALIGN_CENTER_VERTICAL);
  verticalSizer->Add(drawingPanel, 0,
                     wxFIXED_MINSIZE | wxALIGN_CENTER_HORIZONTAL);
  verticalSizer->AddSpacer(30);
  verticalSizer->Add(controlSizer, 0, wxALIGN_CENTER_HORIZONTAL);
  SetSizerAndFit(verticalSizer);
}

void GameFrame::gameLoop()
{
  while (isGameRunning)
  {
    DelayTimer t{delay};
    position->advanceGen();
    drawingPanel->Refresh();
    auto generationText = std::string("Generation: ") +=
        std::to_string(position->getGenCount());
    auto cellAmountText = std::string("Alive cells: ") +=
        std::to_string(position->getCellsQuantity());
    generationLabel->SetLabelText(generationText);
    cellAmountLabel->SetLabelText(cellAmountText);
    wxYield();
    t.wait();
  }
}

void GameFrame::onButtonClick(wxCommandEvent &)
{
  if (isGameRunning)
  {
    startButton->SetLabel("Start");
    isGameRunning = false;
  } else
  {
    isGameRunning = true;
    startButton->SetLabelText("Stop");
    gameLoop();
  }
}

void GameFrame::onClose(wxCloseEvent &)
{
  isGameRunning = false;
  Show(false);
  Destroy();
}