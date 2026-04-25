#include <string_view>

#include <wx/filepicker.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>

#include "../settings.hpp"

class SettingsPanel : public wxPanel
{
public:
  SettingsPanel(wxFrame *parent, Settings &initialSettings);

  Settings getSettingsInput() const;

  std::chrono::milliseconds getDelay() const;

  std::filesystem::path getPosPath() const;

  wxFilePickerCtrl *pathInput;
  wxSpinCtrl *delayInput;

private:
  void createControls(Settings &initialSettings);

  wxStaticText *createLabel(std::string_view labelText);

  void setUpLayout();
};