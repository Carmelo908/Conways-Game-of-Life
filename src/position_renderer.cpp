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
      auto cellPoint = wxPoint(x * zoom, y * zoom);
      bitmapDC.DrawRectangle(cellPoint, wxSize(zoom, zoom));
    }
  }
  return posBitmap;
}

void PositionRenderer::moveCamera(Direction d)
{
  /* Camera move nimplemented */
  switch (d)
  {
  case Direction::up:
    camera.y++;
    break;
  case Direction::down:
    camera.y--;
    break;
  case Direction::left:
    camera.x--;
    break;
  case Direction::right:
    camera.x++;
    break;
  }
}
void PositionRenderer::setZoom(int z) { zoom = z; }

int PositionRenderer::getZoom() const { return zoom; }

void PositionRenderer::setSize(wxSize s) { size = s; }

wxSize PositionRenderer::getSize() const { return size; }
