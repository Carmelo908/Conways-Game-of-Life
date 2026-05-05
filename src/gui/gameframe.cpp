#include "gameframe.hpp"

#include <filesystem>
#include <fstream>
#include <thread>
#include <utility>

#include <wx/button.h>
#include <wx/filepicker.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include <nlohmann/json.hpp>

#include "../position.hpp"
#include "../settings.hpp"
#include "positionpanel.hpp"
#include "settingspanel.hpp"

constexpr auto parsingErrorMessage =
    "An error ocurred parsing the file. It may contain invalid data.";

constexpr auto missingPathFieldMessage =
    "The position file field is missing. Select a file to open";

std::unique_ptr<Position> openPosition(const std::filesystem::path &path)
{
  if (path.empty())
  {
    return nullptr;
  }
  try
  {
    return std::make_unique<Position>(Position::parseJsonFile(path));
  } catch (const nlohmann::json::exception &e)
  {
    wxMessageBox(parsingErrorMessage, "Error opening the file");
    return nullptr;
  }
}

/// @brief Class made to wait a minimun time
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

GameFrame::GameFrame(Settings &settings)
  : wxFrame(nullptr, wxID_ANY, "Conway's Game of Life", wxDefaultPosition),
    position{},
    isGameRunning{false},
    settingsPanel{new SettingsPanel(this, settings)}
{
  Bind(wxEVT_CLOSE_WINDOW, &GameFrame::onClose, this);
  Bind(wxEVT_BUTTON, &GameFrame::onStartButtonClick, this);
  Bind(wxEVT_FILEPICKER_CHANGED, &GameFrame::onPositionChanged, this);
  SetFont(GetFont().Scale(1.3));
  createComponents();
  setUpLayout();
  Show(true);
  Maximize();
}

void GameFrame::createComponents()
{
  startButton = new wxButton(this, wxID_ANY, "Start");
  startButton->Disable();
  positionPanel = new PositionPanel(this);
  generationLabel = new wxStaticText(this, wxID_ANY, "");
  cellsQuantityLabel = new wxStaticText(this, wxID_ANY, "");
}

wxSizer *GameFrame::createControlSizer() const
{
  auto controlSizer{new wxBoxSizer(wxHORIZONTAL)};
  controlSizer->Add(generationLabel, wxALIGN_CENTER_VERTICAL);
  controlSizer->AddSpacer(20);
  controlSizer->Add(startButton, wxALIGN_CENTER_VERTICAL);
  controlSizer->AddSpacer(20);
  controlSizer->Add(cellsQuantityLabel, wxALIGN_CENTER_VERTICAL);
  return controlSizer;
}

void GameFrame::setUpLayout()
{
  auto mainSizer{new wxBoxSizer(wxHORIZONTAL)};
  mainSizer->Add(settingsPanel);
  mainSizer->AddSpacer(20);
  auto centerColumnSizer{new wxBoxSizer(wxVERTICAL)};
  auto controlSizer = createControlSizer();
  centerColumnSizer->Add(positionPanel, 0,
                         wxRESERVE_SPACE_EVEN_IF_HIDDEN | wxFIXED_MINSIZE |
                             wxALIGN_CENTER_HORIZONTAL);
  centerColumnSizer->AddSpacer(20);
  centerColumnSizer->Add(controlSizer, 0, wxALIGN_CENTER_HORIZONTAL);
  mainSizer->Add(centerColumnSizer, 0, wxFIXED_MINSIZE);
  SetSizerAndFit(mainSizer);
}

void GameFrame::gameLoop()
{
  while (isGameRunning)
  {
    DelayTimer t{settings.getDelay()};
    position->advanceGen();
    positionPanel->Refresh();
    updatePositionLabels();
    wxYield();
    t.wait();
  }
}

void GameFrame::onStartButtonClick(wxCommandEvent &)
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

void GameFrame::onPositionChanged(wxFileDirPickerEvent &event)
{
  position = openPosition(event.GetPath().ToStdString());
  if (position == nullptr)
  {
    return;
  }
  updatePositionLabels();
  startButton->Enable();
  positionPanel->showPosition(*position);
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