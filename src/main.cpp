#include <wx/app.h>

#include "gui/settingsmenu.hpp"
#include "settings.hpp"

class App : public wxApp
{
public:
  virtual bool OnInit() override;
};

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
  Settings initialSettings;
  if (std::filesystem::exists("./settings.json"))
  {
    initialSettings = parseFileSettings("./settings.json");
  }
  new SettingsMenu(initialSettings);

  return true;
}