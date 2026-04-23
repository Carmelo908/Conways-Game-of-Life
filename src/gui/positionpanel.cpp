#include "positionpanel.hpp"

#include <wx/dcclient.h>
#include <wx/dcmemory.h>

PositionPanel::PositionPanel(wxWindow *parent, const Position *position)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(1000, 500),
            wxBORDER_THEME)
{
  Bind(wxEVT_PAINT, &PositionPanel::OnPaint, this);
  SetBackgroundColour(wxColour(0, 0, 0));
  int cellWidth, cellHeight;
  cellWidth = maxWidth / position->width;
  cellHeight = maxHeight / position->height;
  int cellSizeUnits = std::min(cellWidth, cellHeight);
  SetClientObject(
      new ClientDrawingData(*position, wxSize(cellSizeUnits, cellSizeUnits)));
  wxSize panelSize{cellSizeUnits * position->width + 1,
                   cellSizeUnits * position->height + 1};
  SetSize(panelSize);
}

void PositionPanel::OnPaint(wxPaintEvent &)
{
  auto clientData{static_cast<ClientDrawingData *>(GetClientObject())};
  auto position{clientData->position};
  auto cellSize{clientData->cellSize};
  auto posBitmap = makePositionBitmap(position, cellSize);
  wxPaintDC(this).DrawBitmap(posBitmap, 0, 0);
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
