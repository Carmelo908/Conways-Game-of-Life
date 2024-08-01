#include <string_view>

#include <wx/filepicker.h>
#include <wx/spinctrl.h>
#include <wx/wx.h>

#include "../settingsdata.hpp"

class FieldsPanel : public wxPanel
{
public:
  FieldsPanel(wxFrame *parent, SettingsData &initialSettings);

  SettingsData getSettingsInput() const;

  std::chrono::milliseconds getDelay() const;

  std::string getPosPath() const;

  wxFilePickerCtrl *pathInput;
  wxSpinCtrl *delayInput;

private:
  void createControls(SettingsData &initialSettings);

  wxStaticText *createLabel(std::string_view labelText);

  void setUpLayout();
};