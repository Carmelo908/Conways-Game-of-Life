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
      auto point = cellPoint(cell);
      bitmapDC.DrawRectangle(point, wxSize(zoom, zoom));
    }
  }
  return posBitmap;
}

void PositionRenderer::moveCamera(Direction d)
{
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
  bool afterLeftEdge = cell.x >= -offsetX();
  bool beforeRightEdge = cell.x * zoom <= size.x - offsetX();
  bool beforeTopEdge = cell.y * zoom <= size.y - offsetY();
  bool afterBottomEdge = cell.y >= -offsetY();
  return beforeTopEdge && beforeRightEdge && afterBottomEdge && afterLeftEdge;
}

wxPoint PositionRenderer::cellPoint(CellCoords cell) const
{
  return wxPoint(cell.x * zoom + offsetX(), cell.y * zoom + offsetY());
}

int PositionRenderer::offsetX() const { return size.x / 2 + camera.x * zoom; }

int PositionRenderer::offsetY() const { return size.y / 2 + camera.y * zoom; }