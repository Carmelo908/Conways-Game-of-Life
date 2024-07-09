#include <thread>
#include <chrono>
#include <fstream>
#include <memory>
#include <string>
#include <atomic>
#include <cinttypes>

#include <position.hpp>

#include <wx/wx.h>
#include <nlohmann/json.hpp>

enum widgets
{
  button = 2,
  mainFrame,
  drawingFrame,
};

class ClientDrawingData : public wxClientData
{
public:
  ClientDrawingData(Position *position, uint8_t cellSize) :
    position {position},
    cellSize {cellSize}
  {};

  Position *position;
  uint8_t cellSize;
};

class DrawingPanel : public wxPanel
{
public:
  DrawingPanel(wxWindow *parent, Position *position) :
    wxPanel(parent, widgets::drawingFrame, wxDefaultPosition, 
    wxSize(1000, 500), wxBORDER_THEME)
  {
    Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);

    SetBackgroundColour(wxColour(0, 0, 0));

    uint8_t cellWidth, cellHeight;

    cellWidth = maxWidth / position->width;
    cellHeight = maxHeight / position->height;

    uint8_t cellSize = cellWidth < cellHeight ? cellWidth : cellHeight;

    wxSize panelSize {cellSize * position->width, cellSize * position->height}; 

    SetClientObject(new ClientDrawingData(position, cellSize));
    SetSize(panelSize);
  }

private:
  void OnPaint(wxPaintEvent &)
  {
    auto clientObject {static_cast<ClientDrawingData*>(GetClientObject())};

    auto position {clientObject->position};
    auto cellSize {clientObject->cellSize};

    wxBitmap posBitmap {1000, 500};
    wxMemoryDC bitmapDC {posBitmap};

    bitmapDC.SetPen(*wxWHITE_PEN);

    for (uint16_t y = 0; y < position->height; y++)
    {
      for (uint16_t x = 0; x < position->width; x++)
      {
        if (!position->getCellAt(x, y)) 
        { 
          continue; 
        };
        bitmapDC.DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize);
      }
    }

    wxPaintDC(this).DrawBitmap(posBitmap, 0, 0);
  };

  static constexpr int maxWidth = 1000;
  static constexpr int maxHeight = 500;
};

class GameFrame : public wxFrame
{
public:
  GameFrame() :
    wxFrame(nullptr, wxID_ANY, "Conway's Game of Life", 
      wxDefaultPosition, wxSize(1000, 700)),
    position {openPosition("./positions/random.json")},
    isGameRunning {false},
    button {new wxButton(this, widgets::button, "Start", 
      wxDefaultPosition, wxSize(100, 100))},
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

private:
  static Position openPosition(std::string filePath)
  {
    std::ifstream jsonFile {filePath};

    const nlohmann::json jsonObject {nlohmann::json::parse(jsonFile)};

    auto openedPosition {jsonObject[0].template get<Position::data_t>()};

    return openedPosition;
  }

  void gameLoop()
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

  void onButtonClick(wxCommandEvent&)
  {
    if (isGameRunning)
    {
      isGameRunning = false;
      return;
    }

    isGameRunning = true;
    std::thread(&GameFrame::gameLoop, this).detach();
  };

  Position position;
  std::atomic<bool> isGameRunning;

  wxButton *button;
  DrawingPanel *drawingPanel;
};