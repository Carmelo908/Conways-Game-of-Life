#include <sys/types.h>
#include <wx/bitmap.h>
#include <wx/gdicmn.h>

#include "position.hpp"

enum Direction
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

  void setSize(int s);
  int getSize() const;

private:
  wxSize size;
  int zoom;
  wxPoint camera;
};