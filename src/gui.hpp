#include <thread>

#include <wx/wx.h>
#include <wx/panel.h>

#include <position.hpp>

enum widgets
{
  button = 2,
  mainFrame,
  canvas,
};

wxDEFINE_EVENT(GAME_START, wxEvent);

class PosClientPtr : public wxClientData
{
  public:
    PosClientPtr(Position* positionPtr) : 
      position {positionPtr}
    {};

  Position *const position;
};

class PosDrawingPanel : public wxPanel
{
public:
  PosDrawingPanel(wxWindow *parent) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(500, 300))
   {
    SetBackgroundColour(wxColour(0, 0, 0));
   }

  void gameLoop(wxCommandEvent &event)
  {
    auto *clientObject = static_cast<PosClientPtr*>(event.GetClientObject());

    clientObject->position->advanceGen();
    Refresh();
  }

  void OnPaint(wxPaintEvent &event)
  {
    auto clientObject {static_cast<PosClientPtr*>(this->GetClientObject())};

    if (!clientObject)
      return;

    wxPaintDC dc {this};
    dc.SetPen(*wxWHITE_PEN);

    dc.Clear();

    for (size_t y = 0; y < clientObject->position->height; y++)
      for (size_t x = 0; x < clientObject->position->width; x++)
        if (clientObject->position->getCellAt(x, y))
          dc.DrawRectangle(x, y, 1, 1);
  };

  wxDECLARE_EVENT_TABLE();
};


class StartButton : public wxButton
{
public:
  StartButton(wxWindow *parent) :
    wxButton(parent, button, "Start", wxPoint(1000, 600), wxSize(100, 100))
  {};

};


class MainFrame : public wxFrame 
{
public:
  MainFrame(Position *currentGenPtr) :
    wxFrame(nullptr, wxID_ANY, wxString("Conway's Life Game"), wxDefaultPosition, wxSize(1000, 700)),
    currentGen {new PosClientPtr(currentGenPtr)},
    posCanvas {new PosDrawingPanel(this)},
    button {new StartButton(this)}
  {
    posCanvas->SetClientObject(currentGen);
    SetBackgroundColour(wxColour(255, 0, 0));
  }

  void onButtonClick(wxCommandEvent& WXUNUSED(event))
  {
    currentGen->position->advanceGen();
    posCanvas->Refresh();
    posCanvas->Update();
  };

  PosClientPtr *currentGen;
  PosDrawingPanel *posCanvas;
  StartButton *button;

  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_BUTTON(widgets::button, MainFrame::onButtonClick)
  EVT_COMMAND(widgets::canvas, GAME_START, PosDrawingPanel::gameLoop)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(PosDrawingPanel, wxPanel)
  EVT_PAINT(PosDrawingPanel::OnPaint)
wxEND_EVENT_TABLE()