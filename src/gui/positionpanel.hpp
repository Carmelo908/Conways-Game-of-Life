#pragma once

#include <wx/panel.h>

class Position;
class wxBitmap;
class wxPaintEvent;

class PositionPanel : public wxPanel
{
public:
  PositionPanel(wxWindow *parent, const Position &position);

private:
  void OnPaint(wxPaintEvent &);

  static int getCellSizeUnits(const Position &pos);

  static wxBitmap makePositionBitmap(const Position &position, wxSize cellSize);

  struct ClientDrawingData : public wxClientData
  {
    ClientDrawingData(const Position &position, wxSize cellSize);

    const Position &position;
    const wxSize cellSize;
  };

  static constexpr int maxWidth = 1000;
  static constexpr int maxHeight = 500;
};