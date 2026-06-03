
#include "position.hpp"

#include <wx/gdicmn.h>

enum class Direction
{
  up,
  down,
  left,
  right
};

class PositionRenderer
{
public:
  PositionRenderer(wxSize size, int zoom = 5, wxPoint camera = {});

  PositionRenderer() = delete;
  PositionRenderer(PositionRenderer &&) = default;
  PositionRenderer(const PositionRenderer &) = default;
  PositionRenderer &operator=(PositionRenderer &&) = default;
  PositionRenderer &operator=(const PositionRenderer &) = default;
  ~PositionRenderer() = default;

  wxBitmap render(const Position &position) const;

  void moveCamera(Direction d);

  void setZoom(int z);
  int getZoom() const;

  void setSize(wxSize s);
  wxSize getSize() const;

private:
  bool insideBitmap(CellCoords cell) const;

  wxSize size;
  int zoom;
  wxPoint camera;
};