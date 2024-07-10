#include <thread>
#include <chrono>
#include <fstream>

#include <nlohmann/json.hpp>

#include "gameframe.hpp"

GameFrame::GameFrame() :
  wxFrame(nullptr, wxID_ANY, "Conway's Game of Life", wxDefaultPosition),
  position {openPosition("./positions/random.json")},
  isGameRunning {false},
  button {new wxButton(this, wxID_ANY, "Start", wxDefaultPosition)},
  drawingPanel {new DrawingPanel(this, &position)}
{
  button->Bind(wxEVT_BUTTON, &GameFrame::onButtonClick, this); 

  wxBoxSizer *boxSizer {new wxBoxSizer(wxVERTICAL)};

  boxSizer->Add(drawingPanel, 0, wxFIXED_MINSIZE | wxALIGN_CENTER_HORIZONTAL);
  boxSizer->Add(button, 0, wxALIGN_CENTER_HORIZONTAL);

  SetSizerAndFit(boxSizer);
  SetBackgroundColour(wxColour(255, 0, 0));
  Show(true);
  Maximize();
}

Position GameFrame::openPosition(std::string filePath)
{
  std::ifstream jsonFile {filePath};

  const nlohmann::json jsonObject {nlohmann::json::parse(jsonFile)};

  auto openedPosition {jsonObject[0].template get<Position::data_t>()};

  return openedPosition;
}

void GameFrame::gameLoop()
{
  for (size_t i = 0; i < 100000; i++)
  {
    using namespace std::chrono;
    auto startTime = high_resolution_clock::now();
    
    drawingPanel->Refresh();
    if (!isGameRunning) 
    {
      break;
    }
    position.advanceGen();

    auto endTime = high_resolution_clock::now();
    auto timeElapsed = duration_cast<milliseconds>(endTime - startTime);
    std::this_thread::sleep_for(milliseconds(10) - timeElapsed);
    // This is for adding configurable delay in a future
  }

  isGameRunning = false;
  };

void GameFrame::onButtonClick(wxCommandEvent &)
{
  if (isGameRunning)
  {
    isGameRunning = false;
    return;
  }

  isGameRunning = true;
  std::thread(&GameFrame::gameLoop, this).detach();
};