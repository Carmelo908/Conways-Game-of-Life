#pragma once

#include <wx/bitmap.h>
#include <wx/event.h>
#include <wx/panel.h>

#include "../position.hpp"

class PositionPanel : public wxPanel
{
public:
  PositionPanel(wxWindow *parent, const Position *const position);

private:
  void OnPaint(wxPaintEvent &);

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