#include <filesystem>

#include <wx/wx.h>

#include "serialization.hpp"
#include "gui/configmenu.hpp"

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