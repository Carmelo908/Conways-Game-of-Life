#include "position_renderer.hpp"
#include "position.hpp"

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
  for (auto cell : position)
  {
    if (insideBitmap(cell))
    {
      auto cellPoint = wxPoint(cell.x * zoom, cell.y * zoom);
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

bool PositionRenderer::insideBitmap(CellCoords cell) const
{
  bool insideCoordsX = cell.x * zoom <= size.x && cell.x >= 0;
  bool insideCoordsY = cell.y * zoom <= size.y && cell.y >= 0;
  return insideCoordsX && insideCoordsY;
}