#include <wx/app.h>

#include "gui/gameframe.hpp"
#include "settings.hpp"

/// @brief Project's App class
class App : public wxApp
{
public:
  /// @brief Entry point of the program.
  ///
  /// It tries to open the settings file if it exists and parse them
  ///
  /// @return Wether or not the program was started sucessfully.
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
  new GameFrame(initialSettings);

  return true;
}