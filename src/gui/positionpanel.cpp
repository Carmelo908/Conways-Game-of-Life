#include "positionpanel.hpp"

#include <wx/bitmap.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/gdicmn.h>

#include "../position.hpp"

PositionPanel::PositionPanel(wxWindow *parent)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(1000, 500),
            wxBORDER_THEME)
{
  Bind(wxEVT_PAINT, &PositionPanel::OnPaint, this);
  SetBackgroundColour(wxColour(0, 0, 0));
  const wxSize panelSize{maxWidth, maxHeight};
  SetSize(panelSize);
}

void PositionPanel::showPosition(Position *position)
{
  SetClientObject(new DrawingData(position));
  Refresh();
}

void PositionPanel::OnPaint(wxPaintEvent &)
{
  auto clientData{dynamic_cast<DrawingData *>(GetClientObject())};
  if (clientData == nullptr || clientData->position == nullptr)
  {
    wxPaintDC(this).Clear();
    return;
  }
  auto posBitmap = makePositionBitmap(*clientData->position);
  wxPaintDC(this).DrawBitmap(posBitmap, 0, 0);
}

wxBitmap PositionPanel::makePositionBitmap(const Position &position)
{
  wxBitmap posBitmap{maxWidth, maxHeight};
  wxMemoryDC bitmapDC{posBitmap};
  bitmapDC.SetPen(*wxWHITE_PEN);
  for (int y = -maxHeight / 2; y < maxHeight / 2; y++)
  {
    for (int x = -maxWidth / 2; x < maxWidth / 2; x++)
    {
      if (!position.getCellAt(x, y))
      {
        continue;
      };
      auto cellPoint = wxPoint(x * 5, y * 5);
      bitmapDC.DrawRectangle(cellPoint, wxSize(5, 5));
    }
  }
  return posBitmap;
}

PositionPanel::DrawingData::DrawingData(const Position *position)
  : position{position}
{}