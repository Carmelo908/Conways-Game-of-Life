#include <thread>
#include <chrono>
#include <exception>
#include <mutex>
#include <string>

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/sizer.h>

#include <position.hpp>

enum widgets
{
  button = 2,
  mainFrame,
  drawingFrame,
};

class ClientPos : public wxClientData
{
public:
  ClientPos(Position *positionPtr) :
    position {positionPtr}
  {};

  Position *position;
};

class ClientDrawingData : public ClientPos
{
public:
  ClientDrawingData(Position *position, uint8_t cellSize) :
    ClientPos {position},
    cellSize {cellSize}
  {};

  uint8_t cellSize;
};

class DrawingPanel : public wxPanel
{
public:
  DrawingPanel(wxWindow *parent, Position *position) :
    wxPanel(parent, widgets::drawingFrame, wxDefaultPosition, 
    wxSize(1000, 500), wxBORDER_THEME)
  {
    SetBackgroundColour(wxColour(0, 0, 0));

    constexpr int maxWidth = 1000;
    constexpr int maxHeight = 500;

    uint8_t cellWidth, cellHeight;

    cellWidth = maxWidth / position->width;
    cellHeight = maxHeight / position->height;

    uint8_t cellSize = cellWidth < cellHeight ? cellWidth : cellHeight;

    wxSize panelSize {cellSize * position->width, cellSize * position->height}; 

    SetClientObject(new ClientDrawingData(position, cellSize));
    SetSize(panelSize);
  }

private:

  void OnPaint(wxPaintEvent &event)
  {
    auto clientObject {static_cast<ClientDrawingData*>(GetClientObject())};

    auto position {clientObject->position};
    auto cellSize {clientObject->cellSize};

    wxPaintDC paintDC {this};

    wxBitmap posBitmap {1000, 500};
    wxMemoryDC bitmapDC {posBitmap};

    bitmapDC.SetPen(*wxWHITE_PEN);

    for (uint16_t y = 0; y < position->height; y++)
    {
      for (uint16_t x = 0; x < position->width; x++)
      {
        if (position->getCellAt(x, y))
        {
          bitmapDC.DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize);
        }
      }
    }

    paintDC.DrawBitmap(posBitmap, 0, 0);
  };

  wxDECLARE_EVENT_TABLE();
};


class StartButton : public wxButton
{
public:
  StartButton(wxWindow *parent) :
    wxButton(parent, widgets::button, "Start", wxPoint(1000, 600), wxSize(100, 100))
  {};

};


class MainFrame : public wxFrame
{
public:
  MainFrame() :
    wxFrame(nullptr, wxID_ANY, wxString("Conway's Life Game"), 
    wxDefaultPosition, wxSize(1000, 700)),
    position {openPosition()},
    isGameRunning {false},
    button {new StartButton(this)},
    drawingPanel {new DrawingPanel(this, position)}
  {
    wxBoxSizer *boxSizer {new wxBoxSizer(wxVERTICAL)};

    boxSizer->Add(drawingPanel, 0, wxALIGN_CENTER_HORIZONTAL, 5);
    // TO DO: fix drawingPanel sizing
    SetSizerAndFit(boxSizer);

    SetBackgroundColour(wxColour(255, 0, 0));
    Show(true);
  }

private:
  Position* openPosition() 
  {
    std::ifstream jsonFile {"./positions/random.json"};

    const nlohmann::json jsonObject {nlohmann::json::parse(jsonFile)};

    Position *openedPosition {new Position(jsonObject[0].template get<Position::data_t>())};

    return openedPosition;
  }

  void gameLoop()
  {
    for (size_t i = 0; i < 100000; i++)
    {
      using namespace std::chrono;
      auto startTime = high_resolution_clock::now();
      
      drawingPanel->Refresh();
      if (!(isGameRunning))
        break;
      position->advanceGen();

      auto endTime = high_resolution_clock::now();

      milliseconds timeElapsed = 
      duration_cast<milliseconds>(endTime - startTime);
      // This is for adding configurable delay in a future
    }

    mtx.lock();
    isGameRunning = false;
    mtx.unlock();
  };

  void onButtonClick(wxCommandEvent& WXUNUSED(event))
  {
    if (isGameRunning)
    {
      mtx.lock();
      isGameRunning = false;
      mtx.unlock();
      return;
    }
    
    isGameRunning = true;

    std::thread gameThread{&MainFrame::gameLoop, this};
    gameThread.detach();
  };

  Position *position;

  DrawingPanel *drawingPanel;

  StartButton *button;

  std::mutex mtx;

  bool isGameRunning;

  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_BUTTON(widgets::button, MainFrame::onButtonClick)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
  EVT_PAINT(DrawingPanel::OnPaint)
wxEND_EVENT_TABLE()