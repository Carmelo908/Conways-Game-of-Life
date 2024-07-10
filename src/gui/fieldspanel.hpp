#include <wx/wx.h>
#include <wx/spinctrl.h>

#include "settingsdata.hpp"

class FieldsPanel : public wxPanel
{
public:
  FieldsPanel(wxFrame *parent);

  wxTextCtrl *pathInput;
  wxSpinCtrl *delayInput;

private:
  static constexpr int gridRows = 2;
  static constexpr int gridColumns = 2;
};
