#pragma once

#include <wx/wx.h>

#include "../position.hpp"

class DrawingPanel : public wxPanel
{
public:
  DrawingPanel(wxWindow *parent, const Position *const position);

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