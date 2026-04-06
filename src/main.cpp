#include <filesystem>

#include <wx/wx.h>

#include "gui/configmenu.hpp"
#include "serialization.hpp"

class MyApp : public wxApp
{
public:
  virtual bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
  SettingsData initialSettings;
  if (std::filesystem::exists("./settings.toml"))
  {
    initialSettings = parseFileSettings("./settings.toml");
  }
  new ConfigMenu(initialSettings);

  return true;
}