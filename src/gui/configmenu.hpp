#include <string>
#include <chrono>
#include <fstream>

#include <wx/wx.h>

#include <gui/gameframe.hpp>
#include <tomlconfig/settingsdata.hpp>

class SettingsClientData : private wxClientData
{
public:
  SettingsClientData(SettingsData& settings) : 
    settings(settings) 
  {};

  SettingsData &settings;
};

class ConfigMenu : private wxFrame
{
public:
  ConfigMenu() :
    wxFrame(nullptr, wxID_ANY, "Start program"),
    pathInput {new wxTextCtrl(this, wxID_ANY)},
    delayInput {new wxTextCtrl(this, wxID_ANY)}
  {
    SetMaxSize(wxSize(400, 1000));

    constexpr int gridRows = 3;
    constexpr int gridColumns = 2;

    wxGridSizer *grid = new wxGridSizer(gridRows, gridColumns, 0, 0);

    grid->Add(pathInput);
    grid->Add(delayInput);

    SetSizer(grid);
    Show(true);
  }

private:
  void OnAcceptButton(wxCloseEvent)
  {
    Destroy();
    new GameFrame();
  }

  wxTextCtrl *pathInput;
  wxTextCtrl *delayInput;
};