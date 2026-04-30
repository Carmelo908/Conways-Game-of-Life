#include "positionpanel.hpp"

#include <wx/bitmap.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>

#include "../position.hpp"

PositionPanel::PositionPanel(wxWindow *parent)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(1000, 500),
            wxBORDER_THEME)
{
  Bind(wxEVT_PAINT, &PositionPanel::OnPaint, this);
  SetBackgroundColour(wxColour(0, 0, 0));
  Hide();
}

void PositionPanel::showPosition(Position &position)
{
  int cellSizeUnits = getCellSizeUnits(position);
  auto cellDisplaySize = wxSize(cellSizeUnits, cellSizeUnits);
  SetClientObject(new ClientDrawingData(position, cellDisplaySize));
  wxSize panelSize{cellSizeUnits * position.width + 1,
                   cellSizeUnits * position.height + 1};
  SetMinSize(panelSize);
  Show();
}

void PositionPanel::OnPaint(wxPaintEvent &)
{
  auto clientData{static_cast<ClientDrawingData *>(GetClientObject())};
  auto posBitmap =
      makePositionBitmap(clientData->position, clientData->cellSize);
  wxPaintDC(this).DrawBitmap(posBitmap, 0, 0);
}

int PositionPanel::getCellSizeUnits(const Position &pos)
{
  int cellWidth, cellHeight;
  cellWidth = maxWidth / pos.width;
  cellHeight = maxHeight / pos.height;
  return std::min(cellWidth, cellHeight);
}

wxBitmap PositionPanel::makePositionBitmap(const Position &position,
                                           wxSize cellSize)
{
  wxBitmap posBitmap{maxWidth, maxHeight};
  wxMemoryDC bitmapDC{posBitmap};
  bitmapDC.SetPen(*wxWHITE_PEN);
  for (int y = 0; y < position.height; y++)
  {
    for (int x = 0; x < position.width; x++)
    {
      if (!position.getCellAt(x, y))
      {
        continue;
      };
      auto cellPoint = wxPoint(x * cellSize.GetX(), y * cellSize.GetY());
      bitmapDC.DrawRectangle(cellPoint, cellSize);
    }
  }
  return posBitmap;
}

PositionPanel::ClientDrawingData::ClientDrawingData(const Position &position,
                                                    wxSize cellSize)
  : position{position},
    cellSize{cellSize}
{}
