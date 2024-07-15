#include <fstream>
#include <utility>

#include <nlohmann/json.hpp>

#include "configmenu.hpp"
#include "gameframe.hpp"

ConfigMenu::ConfigMenu() :
  wxFrame(nullptr, wxID_ANY, "Settings"),
  fieldsPanel {new FieldsPanel(this)}
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
  wxBoxSizer *verticalSizer {new wxBoxSizer(wxVERTICAL)};
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
  std::ifstream jsonFile {filePath.data()};
  const nlohmann::json jsonObject {nlohmann::json::parse(jsonFile)};

  auto openedPosition {std::make_unique<Position>
  (jsonObject[0].template get<Position::data_t>())};

  return openedPosition;
};

void ConfigMenu::OnAcceptButton(wxCommandEvent &)
{
  SettingsData settings;
  settings = fieldsPanel->getSettingsInput();
  if (settings.getUnopenedPath().empty())
  {
    wxMessageBox("No file has been selected", "Empty required field");
    return;
  }
  try
  {
    settings.position = openPosition(settings.getUnopenedPath());  
  }
  catch (const nlohmann::json::parse_error &)
  {
    wxMessageBox("Error parsing the file; it may contain invalid data.",
                  "Parsing error");
    return;
  }
  Show(false);
  Close();
  new GameFrame(settings);
}