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
  bool OnInit() override;
};

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
  if (std::filesystem::exists("./settings.json"))
  {
    auto a = std::make_unique<Settings>(parseFileSettings("./settings.json"));
    new GameFrame(std::move(a));
  } else
  {
    new GameFrame(std::make_unique<Settings>());
  }

  return true;
}