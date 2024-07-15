#include <string_view>

#include <wx/filepicker.h>
#include <wx/spinctrl.h>
#include <wx/wx.h>

#include "../settingsdata.hpp"

class FieldsPanel : public wxPanel
{
public:
  FieldsPanel(wxFrame *parent);

  SettingsData getSettingsInput() const;

  wxFilePickerCtrl *pathInput;
  wxSpinCtrl *delayInput;

private:
  void createControls();

  wxStaticText *createLabel(std::string_view labelText);

  void setUpLayout();

  std::chrono::milliseconds getDelay() const;

  std::string getPosPath() const;
};
