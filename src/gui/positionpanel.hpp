#pragma once

#include <wx/panel.h>

class Position;
class wxBitmap;
class wxPaintEvent;

/// @brief Panel which draws the position on itself.
class PositionPanel : public wxPanel
{
public:
  /// @brief Constructs the panel which is hidden until showPosition is
  /// executed at least once.
  /// @param parent: the parent windows (GameFrame).
  PositionPanel(wxWindow *parent);

  /// @brief shows the Position on the panel in a monochromatic wxBitmap every
  /// time a wxPaintEvent is processed.
  ///
  /// If it's the first time this method was called since widget creation,
  /// unhides it
  ///
  /// @param position: the Position to be shown.
  void showPosition(Position &position);

private:
  void OnPaint(wxPaintEvent &);

  static int getCellSizeUnits(const Position &pos);

  static wxBitmap makePositionBitmap(const Position &position, wxSize cellSize);

  struct ClientDrawingData : public wxClientData
  {
    ClientDrawingData(const Position &position, wxSize cellSize);

    const Position &position;
    const wxSize cellSize;
  };

  static constexpr int maxWidth = 1000;
  static constexpr int maxHeight = 500;
};