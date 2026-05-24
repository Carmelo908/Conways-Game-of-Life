#include "gameframe.hpp"

#include <chrono>
#include <filesystem>

#include <memory>
#include <wx/button.h>
#include <wx/filepicker.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/timer.h>

#include <nlohmann/json.hpp>

#include "../game_manager.hpp"
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
    gameManager{std::make_unique<GameManager>()},
    delay{initialSettings->getDelay()},
    gameTimer{new wxTimer(this)},
    positionPanel{new PositionPanel(this)}
{
  assert(initialSettings != nullptr);
  settingsPanel = new SettingsPanel(this, *initialSettings);
  Bind(wxEVT_CLOSE_WINDOW, &GameFrame::onClose, this);
  Bind(wxEVT_BUTTON, &GameFrame::onStartButtonClick, this);
  Bind(wxEVT_TIMER, &GameFrame::onGameTimer, this);
  Bind(EVT_SETTINGS_UPDATED, &GameFrame::onSettingsChanged, this);
  SetFont(GetFont().Scale(1.3));
  createComponents();
  changePosition(openPosition(initialSettings->getPositionPath()));
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
  namespace chr = std::chrono;

  auto start = chr::steady_clock::now();
  const std::shared_ptr position = gameManager->yieldPosition();
  positionPanel->showPosition(position);
  updatePositionLabels(*position);
  auto elapsed = chr::steady_clock::now() - start;
  auto elapsedms = chr::duration_cast<chr::milliseconds>(elapsed).count();
  gameTimer->StartOnce(delay.count() - elapsedms);
}

void GameFrame::onStartButtonClick(wxCommandEvent &)
{
  if (gameTimer->IsRunning())
  {
    startButton->SetLabelText("Start");
    gameTimer->Stop();
  } else
  {
    gameTimer->StartOnce(delay.count());
    startButton->SetLabelText("Stop");
  }
}

void GameFrame::onSettingsChanged(SettingsUpdateEvent &e)
{
  auto settings = e.getSettings();
  if (e.hasPositionChanged())
  {
    changePosition(openPosition(settings->getPositionPath()));
  }
}

void GameFrame::changePosition(std::unique_ptr<Position> &&newPosition)
{
  gameTimer->Stop();
  gameManager->reset(std::move(newPosition));
  if (gameManager->empty())
  {
    return;
  }
  startButton->Enable();
  const std::shared_ptr position = gameManager->yieldPosition();
  updatePositionLabels(*position);
  positionPanel->showPosition(position);
}

void GameFrame::updatePositionLabels(const Position &position)
{
  auto value = std::to_string(position.getGenCount());
  auto generationText = std::string("Generation: ") + value;
  generationLabel->SetLabelText(generationText);
  value = std::to_string(position.getCellsQuantity());
  auto cellsQuantityText = std::string("Cells amount: ") + value;
  cellsQuantityLabel->SetLabelText(cellsQuantityText);
}

void GameFrame::onClose(wxCloseEvent &)
{
  saveSettings(settingsPanel->getSettings());
  Show(false);
  Destroy();
}