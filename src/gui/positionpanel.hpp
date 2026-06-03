#pragma once

#include <wx/panel.h>

class Position;
class PositionRenderer;
class wxBitmap;
class wxPaintEvent;

/// @brief Panel which draws the position on itself.
class PositionPanel : public wxPanel
{
public:
  /// @brief Constructs the panel which is hidden until showPosition is
  /// executed at least once.
  /// @param parent: the parent windows (GameFrame).
  PositionPanel(wxWindow *parent, const PositionRenderer *renderer);

  /// @brief shows the Position on the panel in a monochromatic wxBitmap
  ///
  /// @param position: the Position to be shown. if null, the panel just get
  /// black
  void showPosition(std::shared_ptr<const Position> position);

  static wxSize size();

private:
  void OnPaint(wxPaintEvent &);

  struct DrawingData : public wxClientData
  {
    DrawingData(const PositionRenderer *renderer);
    DrawingData(std::shared_ptr<const Position> &position,
                const PositionRenderer *renderer);

    std::shared_ptr<const Position> position;
    const PositionRenderer *renderer;
  };
};