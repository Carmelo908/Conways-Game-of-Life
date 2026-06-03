#include "gameframe.hpp"

#include <filesystem>
#include <memory>

#include <wx/button.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/timer.h>

#include "../game_manager.hpp"
#include "../position_renderer.hpp"
#include "../settings.hpp"
#include "positionpanel.hpp"
#include "settingspanel.hpp"

constexpr auto parsingErrorMessage =
    "An error ocurred parsing the file. It may contain invalid data.";

namespace chr = std::chrono;

static int calcWaitTime(chr::steady_clock::time_point start,
                        chr::milliseconds delay)
{
  auto end = chr::steady_clock::now();
  auto elapsed = chr::duration_cast<chr::milliseconds>(end - start);
  return std::max(delay.count() - elapsed.count(), 1L);
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
    renderer{std::make_unique<PositionRenderer>(PositionPanel::size())},
    gameTimer{new wxTimer(this)},
    positionPanel{new PositionPanel(this, renderer.get())}
{
  assert(initialSettings != nullptr);
  delay = initialSettings->getDelay(),
  settingsPanel = new SettingsPanel(this, *initialSettings);
  Bind(wxEVT_CLOSE_WINDOW, &GameFrame::onClose, this);
  Bind(wxEVT_BUTTON, &GameFrame::onStartButtonClick, this);
  Bind(wxEVT_TIMER, &GameFrame::onGameTimer, this);
  Bind(EVT_SETTINGS_UPDATED, &GameFrame::onSettingsChanged, this);
  SetFont(GetFont().Scale(1.3));
  createComponents();
  updateSettings(*initialSettings);
  changePosition(openNewPosition(initialSettings->getPositionPath()));
  Show(true);
  Maximize();
  SetSizerAndFit(new Layout(this));
}

void GameFrame::createComponents()
{
  startButton = new wxButton(this, wxID_ANY, "Start");
  startButton->Disable();
  generationLabel = new wxStaticText(this, wxID_ANY, "");
  cellsQuantityLabel = new wxStaticText(this, wxID_ANY, "");
}

void GameFrame::updateSettings(Settings &settings)
{
  delay = settings.getDelay();
  renderer->setZoom(settings.getZoom());
}

void GameFrame::onGameTimer(wxTimerEvent &)
{
  auto start = std::chrono::steady_clock::now();
  const std::shared_ptr position = gameManager->yieldPosition();
  positionPanel->showPosition(position);
  updatePositionLabels(*position);
  auto waitTime = calcWaitTime(start, delay);
  gameTimer->StartOnce(waitTime);
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

void GameFrame::onSettingsChanged(SettingsUpdateEvent &event)
{
  auto settings = event.getSettings();
  if (event.hasPositionChanged())
  {
    changePosition(openNewPosition(settings->getPositionPath()));
  }
  updateSettings(*settings);
}

void GameFrame::changePosition(std::unique_ptr<Position> &&newPosition)
{
  gameTimer->Stop();
  if (newPosition == nullptr)
  {
    return;
  }
  gameManager->reset(std::move(newPosition));
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

std::unique_ptr<Position>
GameFrame::openNewPosition(const std::filesystem::path &path)
{
  auto initialCellset{Position::parseJsonFile(path)};
  if (initialCellset.has_value())
  {
    return std::make_unique<Position>(std::move(*initialCellset));
  } else
  {
    wxMessageBox(parsingErrorMessage, "Error opening the file");
    return nullptr;
  }
}

void GameFrame::onClose(wxCloseEvent &)
{
  saveSettings(settingsPanel->getSettings());
  Show(false);
}