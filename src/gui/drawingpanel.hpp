#include <wx/wx.h>

class DrawingPanel : public wxPanel
{
public:
  DrawingPanel(wxWindow *parent, Position *position)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(1000, 500),
              wxBORDER_THEME)
  {
    Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
    SetBackgroundColour(wxColour(0, 0, 0));

    uint8_t cellWidth, cellHeight;
    cellWidth = maxWidth / position->width;
    cellHeight = maxHeight / position->height;
    uint8_t cellSize = cellWidth < cellHeight ? cellWidth : cellHeight;

    wxSize panelSize{cellSize * position->width, cellSize * position->height};
    SetClientObject(new ClientDrawingData(position, cellSize));
    SetSize(panelSize);
  }

private:
  void OnPaint(wxPaintEvent &)
  {
    auto clientObject{static_cast<ClientDrawingData *>(GetClientObject())};

    auto position{clientObject->position};
    auto cellSize{clientObject->cellSize};

    wxBitmap posBitmap{1000, 500};
    wxMemoryDC bitmapDC{posBitmap};

    bitmapDC.SetPen(*wxWHITE_PEN);

    for (uint16_t y = 0; y < position->height; y++)
    {
      for (uint16_t x = 0; x < position->width; x++)
      {
        if (!position->getCellAt(x, y))
        {
          continue;
        };
        bitmapDC.DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize);
      }
    }

    wxPaintDC(this).DrawBitmap(posBitmap, 0, 0);
  }

  struct ClientDrawingData : public wxClientData
  {
    ClientDrawingData(Position *position, uint8_t cellSize)
      : position{position},
        cellSize{cellSize} {};

    Position *position;
    uint8_t cellSize;
  };

  static constexpr int maxWidth = 1000;
  static constexpr int maxHeight = 500;
};