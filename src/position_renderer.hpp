
#include "position.hpp"

#include <wx/gdicmn.h>

enum class Direction
{
  up,
  down,
  left,
  right,
};

class PositionRenderer
{
public:
  /// @param size: the size of the bitmaps that will be rendered.
  /// @param zoom: determines how much zoom will the rendered bitmaps have.
  /// @param camera: the center of the bitmap.
  PositionRenderer(wxSize size, int zoom = 5, wxPoint camera = {});

  PositionRenderer() = delete;
  PositionRenderer(PositionRenderer &&) = default;
  PositionRenderer(const PositionRenderer &) = default;
  PositionRenderer &operator=(PositionRenderer &&) = default;
  PositionRenderer &operator=(const PositionRenderer &) = default;
  ~PositionRenderer() = default;

  /// @return a bitmap with the position drawed in it. its size, zoom and the
  /// coordinates rendered depend on the PositionRenderer's state.
  wxBitmap render(const Position &position) const;

  /// @brief displaces the subarea of the position drawed in the bitmap to
  /// another coordinate when render method is called.
  ///
  /// @param d a value of the enum class Direction, the perspective changes
  /// relative to the current coordinate.
  void moveCamera(Direction d);

  /// @brief displaces the subarea of the position drawed in the bitmap
  /// absolutely to center the coordinate
  void moveCameraAbsolute(CellCoords c);

  void setZoom(int z);
  int getZoom() const;

  void setSize(wxSize s);
  wxSize getSize() const;

private:
  bool insideBitmap(CellCoords cell) const;
  wxPoint cellPoint(CellCoords cell) const;

  int offsetX() const;
  int offsetY() const;

  wxSize size;
  int zoom;
  wxPoint camera;
};