#include "position_renderer.hpp"

#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/gdicmn.h>

PositionRenderer::PositionRenderer(wxSize size, int zoom, wxPoint camera)
  : size{size},
    zoom{zoom},
    camera{camera}
{}

wxBitmap PositionRenderer::render(const Position &position) const
{
  wxBitmap posBitmap{size};
  wxMemoryDC bitmapDC{posBitmap};
  bitmapDC.SetPen(*wxWHITE_PEN);
  for (int y = -size.GetY() / 2; y < size.GetY() / 2; y++)
  {
    for (int x = -size.GetX() / 2; x < size.GetX() / 2; x++)
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