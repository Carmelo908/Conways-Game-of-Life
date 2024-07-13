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

void ConfigMenu::OnAcceptButton(wxCommandEvent &)
{
  SettingsData settings;
  try
  {
    settings = fieldsPanel->getSettingsInput();
  }
  catch(const std::invalid_argument &e)
  {
    wxMessageBox(e.what(), "Error");
    return;
  }
  
  Show(false);
  Destroy();
  new GameFrame();
}