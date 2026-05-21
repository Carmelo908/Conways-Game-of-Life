#include "gameframe.hpp"

#include <filesystem>

#include <wx/button.h>
#include <wx/filepicker.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/timer.h>

#include <nlohmann/json.hpp>

#include "../position.hpp"
#include "../settings.hpp"
#include "positionpanel.hpp"
#include "settingspanel.hpp"

constexpr auto parsingErrorMessage =
    "An error ocurred parsing the file. It may contain invalid data.";

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

class GameFrame::Layout : public wxBoxSizer
{
public:
  explicit Layout(GameFrame *frame)
    : wxBoxSizer(wxHORIZONTAL)
  {
    Add(frame->settingsPanel);
    AddSpacer(20);
    auto centerColumnSizer{new wxBoxSizer(wxVERTICAL)};
    auto controlSizer = createControlSizer(frame);
    centerColumnSizer->Add(frame->positionPanel, PositionPanelFlags());
    centerColumnSizer->AddSpacer(20);
    centerColumnSizer->Add(controlSizer, 0, wxALIGN_CENTER_HORIZONTAL);
    Add(centerColumnSizer, 0, wxFIXED_MINSIZE);
  }

private:
  static wxSizer *createControlSizer(GameFrame *frame)
  {
    auto controlSizer{new wxBoxSizer(wxHORIZONTAL)};
    controlSizer->Add(frame->generationLabel, wxALIGN_CENTER_VERTICAL);
    controlSizer->AddSpacer(20);
    controlSizer->Add(frame->startButton, wxALIGN_CENTER_VERTICAL);
    controlSizer->AddSpacer(20);
    controlSizer->Add(frame->cellsQuantityLabel, wxALIGN_CENTER_VERTICAL);
    return controlSizer;
  }

  static wxSizerFlags PositionPanelFlags()
  {
    return wxSizerFlags().Align(wxALIGN_CENTER_HORIZONTAL).FixedMinSize();
  }
};

GameFrame::GameFrame(std::unique_ptr<Settings> &&initialSettings)
  : wxFrame(nullptr, wxID_ANY, "Conway's Game of Life", wxDefaultPosition),
    settings{std::move(initialSettings)},
    gameTimer{new wxTimer(this)},
    positionPanel{new PositionPanel(this)}
{
  assert(settings != nullptr);
  settingsPanel = new SettingsPanel(this, *settings);
  Bind(wxEVT_CLOSE_WINDOW, &GameFrame::onClose, this);
  Bind(wxEVT_BUTTON, &GameFrame::onStartButtonClick, this);
  Bind(wxEVT_TIMER, &GameFrame::onGameTimer, this);
  Bind(EVT_SETTINGS_UPDATED, &GameFrame::onSettingsChanged, this);
  SetFont(GetFont().Scale(1.3));
  createComponents();
  SetSizerAndFit(new Layout(this));
  Show(true);
  Maximize();
}

void GameFrame::createComponents()
{
  startButton = new wxButton(this, wxID_ANY, "Start");
  startButton->Disable();
  generationLabel = new wxStaticText(this, wxID_ANY, "");
  cellsQuantityLabel = new wxStaticText(this, wxID_ANY, "");
}

void GameFrame::onGameTimer(wxTimerEvent &)
{
  position->advanceGen();
  positionPanel->showPosition(position.get());
  updatePositionLabels();
  gameTimer->StartOnce(settings->getDelay().count());
}

void GameFrame::onStartButtonClick(wxCommandEvent &)
{
  if (gameTimer->IsRunning())
  {
    startButton->SetLabelText("Start");
    gameTimer->Stop();
  } else
  {
    gameTimer->StartOnce(settings->getDelay().count());
    startButton->SetLabelText("Stop");
  }
}

void GameFrame::onSettingsChanged(SettingsUpdateEvent &e)
{
  auto newSettings = e.get();
  if (settings->getPositionPath() != newSettings->getPositionPath())
  {
    changePosition(openPosition(settings->getPositionPath()));
  }
  settings = std::move(newSettings);
}

void GameFrame::changePosition(std::unique_ptr<Position> &&newPosition)
{
  position = std::move(newPosition);
  if (position == nullptr)
  {
    return;
  }
  updatePositionLabels();
  startButton->Enable();
  positionPanel->showPosition(position.get());
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
  saveSettings(*settings);
  Show(false);
  Destroy();
}