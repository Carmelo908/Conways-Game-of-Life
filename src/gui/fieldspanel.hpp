#include <wx/wx.h>
#include <wx/spinctrl.h>

#include "../settingsdata.hpp"

class FieldsPanel : public wxPanel
{
public:
  FieldsPanel(wxFrame *parent);

  struct InvalidInput : public std::runtime_error
  {
    InvalidInput(const std::string& message) : std::runtime_error(message) {}
  };

  SettingsData getSettingsInput() const;

  wxTextCtrl *pathInput;
  wxSpinCtrl *delayInput;

private:
  std::chrono::milliseconds getDelay() const;

  std::string getPosPath() const;

  static constexpr int gridRows = 2;
  static constexpr int gridColumns = 2;
};
