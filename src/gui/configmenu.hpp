#include <string>
#include <chrono>
#include <fstream>

#include <wx/wx.h>
#include <wx/spinctrl.h>

#include <gui/gameframe.hpp>
#include <settingsdata.hpp>

class SettingsClientData : public wxClientData
{
public:
  SettingsClientData(SettingsData *settings) : 
    settings {settings}
  {};

  SettingsData *settings;
};

class FieldsPanel : public wxPanel
{
public:
  FieldsPanel(wxFrame *parent) :
    wxPanel(parent),
    pathInput {new wxTextCtrl(this, wxID_ANY, "./positions/position.json", 
    wxDefaultPosition, wxSize(200, 30))},
    delayInput {new wxSpinCtrl(this, wxID_ANY, '0', 
    wxDefaultPosition, wxSize(200, 30))}
  {
    pathInput->SetFont(pathInput->GetFont().Scale(1.1));
    delayInput->SetFont(delayInput->GetFont().Scale(1.1));

    wxGridSizer *fieldsGrid = new wxGridSizer(gridRows, gridColumns, 10, 100);

    auto pathLabel = new wxStaticText(this, wxID_ANY, "Position file's path:");
    pathLabel->SetFont(pathLabel->GetFont().Scale(1.1));
    fieldsGrid->Add(pathLabel);
    auto delayLabel = new wxStaticText(this, wxID_ANY, 
    "Delay between generations\n(in milliseconds):");
    delayLabel->SetFont(delayLabel->GetFont().Scale(1.1));
    fieldsGrid->Add(delayLabel);
    fieldsGrid->Add(pathInput);
    fieldsGrid->Add(delayInput);

    SetSizerAndFit(fieldsGrid);
  }

  wxTextCtrl *pathInput;
  wxSpinCtrl *delayInput;

private:
  static constexpr int gridRows = 2;
  static constexpr int gridColumns = 2;
};

class ConfigMenu : private wxFrame
{
public:
  ConfigMenu() :
    wxFrame(nullptr, wxID_ANY, "Settings"),
    acceptButton {new wxButton(this, wxID_ANY, "Accept", 
      wxDefaultPosition, wxSize(180, 50))},
    fieldsPanel {new FieldsPanel(this)}
  {
    acceptButton->Bind(wxEVT_BUTTON, &ConfigMenu::OnAcceptButton, this);
    acceptButton->SetFont(acceptButton->GetFont().Scale(1.15));

    wxBoxSizer *verticalSizer {new wxBoxSizer(wxVERTICAL)};
    verticalSizer->AddSpacer(20);
    verticalSizer->Add(fieldsPanel, 0, wxALIGN_CENTER_HORIZONTAL);
    verticalSizer->AddSpacer(80);
    verticalSizer->Add(acceptButton, 0, wxALIGN_CENTER_HORIZONTAL);
    verticalSizer->AddSpacer(20);
    SetSizerAndFit(verticalSizer);

    const wxSize windowSize = GetSize() + wxSize(60, 0);
    SetMinSize(windowSize);
    SetMaxSize(windowSize);
    Show(true);
  }

private:
  void OnAcceptButton(wxCommandEvent &)
  {
    Show(false);
    Destroy();
    new GameFrame();
  }
  
  wxButton *acceptButton;
  FieldsPanel *fieldsPanel;
};