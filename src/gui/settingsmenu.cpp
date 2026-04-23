#include "settingsmenu.hpp"

#include <fstream>

#include <nlohmann/json.hpp>
#include <wx/msgdlg.h>

#include "gameframe.hpp"

constexpr auto parsingErrorMessage =
    "An error ocurred parsing the file. It may contain invalid data.";

constexpr auto missingPathFieldMessage =
    "The position file field is missing. Select a file to open";

SettingsMenu::SettingsMenu(Settings &initialSettings)
  : wxFrame(nullptr, wxID_ANY, "Settings"),
    fieldsPanel{new FieldsPanel(this, initialSettings)}
{
  createButton();
  setUpLayout();
  const wxSize windowSize = GetSize() + wxSize(60, 0);
  SetMinSize(windowSize);
  SetMaxSize(windowSize);
  Show(true);
}

void SettingsMenu::setUpLayout()
{
  wxBoxSizer *verticalSizer{new wxBoxSizer(wxVERTICAL)};
  verticalSizer->AddSpacer(20);
  verticalSizer->Add(fieldsPanel, 0, wxALIGN_CENTER_HORIZONTAL);
  verticalSizer->AddSpacer(80);
  verticalSizer->Add(acceptButton, 0, wxALIGN_CENTER_HORIZONTAL);
  verticalSizer->AddSpacer(20);
  SetSizerAndFit(verticalSizer);
}

void SettingsMenu::createButton()
{
  acceptButton = new wxButton(this, wxID_ANY, "Accept");
  acceptButton->SetInitialSize(wxSize(180, 50));
  acceptButton->Bind(wxEVT_BUTTON, &SettingsMenu::OnAcceptButton, this);
  acceptButton->SetFont(acceptButton->GetFont().Scale(1.15));
}

void SettingsMenu::OnAcceptButton(wxCommandEvent &)
{
  if (fieldsPanel->getPosPath() == "")
  {
    wxMessageBox(missingPathFieldMessage, "Missing required field");
    return;
  }
  Settings settings{fieldsPanel->getSettingsInput()};
  try
  {
    new GameFrame(settings, openPosition(settings.positionPath));
  } catch (const nlohmann::json::parse_error &e)
  {
    wxMessageBox(parsingErrorMessage, "Error opening the file");
    return;
  }
  saveSettings(settings, "settings.json");
  Close();
}