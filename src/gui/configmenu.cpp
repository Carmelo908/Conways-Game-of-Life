#include <fstream>
#include <utility>

#include <nlohmann/json.hpp>
#include <toml++/toml.hpp>

#include "../serialization.hpp"
#include "configmenu.hpp"
#include "gameframe.hpp"

constexpr auto parsingErrorMessage =
    "An error ocurred parsing the file. It may contain invalid data.";

constexpr auto missingPathFieldMessage =
    "The position file field is missing. Select a file to open";

ConfigMenu::ConfigMenu()
  : wxFrame(nullptr, wxID_ANY, "Settings"),
    fieldsPanel{new FieldsPanel(this)}
{
  createButton();
  setUpLayout();
  const wxSize windowSize = GetSize() + wxSize(60, 0);
  SetMinSize(windowSize);
  SetMaxSize(windowSize);
  Show(true);
}

void ConfigMenu::setUpLayout()
{
  wxBoxSizer *verticalSizer{new wxBoxSizer(wxVERTICAL)};
  verticalSizer->AddSpacer(20);
  verticalSizer->Add(fieldsPanel, 0, wxALIGN_CENTER_HORIZONTAL);
  verticalSizer->AddSpacer(80);
  verticalSizer->Add(acceptButton, 0, wxALIGN_CENTER_HORIZONTAL);
  verticalSizer->AddSpacer(20);
  SetSizerAndFit(verticalSizer);
}

void ConfigMenu::createButton()
{
  acceptButton = new wxButton(this, wxID_ANY, "Accept");
  acceptButton->SetInitialSize(wxSize(180, 50));
  acceptButton->Bind(wxEVT_BUTTON, &ConfigMenu::OnAcceptButton, this);
  acceptButton->SetFont(acceptButton->GetFont().Scale(1.15));
}

std::unique_ptr<Position> ConfigMenu::openPosition(std::string_view filePath)
{
  std::ifstream jsonFile{filePath.data()};
  const nlohmann::json jsonObject{nlohmann::json::parse(jsonFile)};
  std::unique_ptr<Position> openedPosition;
  openedPosition = std::make_unique<Position>(
      jsonObject[0].template get<Position::data_t>());
  return openedPosition;
}

void ConfigMenu::OnAcceptButton(wxCommandEvent &)
{
  if (fieldsPanel->getPosPath() == "")
  {
    wxMessageBox(missingPathFieldMessage, "Missing required field");
    return;
  }
  SettingsData settings{fieldsPanel->getSettingsInput()};
  try
  {
    new GameFrame(settings, openPosition(settings.positionPath));
  } catch (const nlohmann::json::parse_error &e)
  {
    wxMessageBox(parsingErrorMessage, "Error opening the file");
    return;
  }
  saveSettings(settings);
  Close();
}