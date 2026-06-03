#include "positionpanel.hpp"
#include "../position_renderer.hpp"

#include <wx/dcclient.h>
#include <wx/gdicmn.h>
#include <wx/gtk/bitmap.h>

PositionPanel::PositionPanel(wxWindow *parent, const PositionRenderer *renderer)
  : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(1000, 500),
            wxBORDER_THEME)
{
  SetClientObject(new DrawingData(renderer));
  Bind(wxEVT_PAINT, &PositionPanel::OnPaint, this);
  SetBackgroundColour(wxColour(0, 0, 0));
  SetSize(size());
}

void PositionPanel::showPosition(std::shared_ptr<const Position> position)
{
  auto clientData = dynamic_cast<DrawingData *>(GetClientObject());
  clientData->position = std::move(position);
  SetClientObject(new DrawingData(clientData->position, clientData->renderer));
  Refresh();
}

wxSize PositionPanel::size() { return {500, 500}; }

void PositionPanel::OnPaint(wxPaintEvent &)
{
  auto clientData{dynamic_cast<DrawingData *>(GetClientObject())};
  if (!clientData || !clientData->position)
  {
    wxPaintDC(this).Clear();
    return;
  }
  auto posBitmap = clientData->renderer->render(*clientData->position);
  wxPaintDC(this).DrawBitmap(posBitmap, 0, 0);
}

PositionPanel::DrawingData::DrawingData(const PositionRenderer *renderer)
  : renderer{renderer}
{}

PositionPanel::DrawingData::DrawingData(
    std::shared_ptr<const Position> &position, const PositionRenderer *renderer)
  : position{position},
    renderer{renderer}
{}