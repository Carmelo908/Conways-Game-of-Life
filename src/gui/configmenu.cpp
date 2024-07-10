#include "configmenu.hpp"

ConfigMenu::ConfigMenu() :
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

void ConfigMenu::OnAcceptButton(wxCommandEvent &)
{
  SettingsData settings;
  try
  {
    settings = fieldsPanel->getSettingsInput();
  }
  catch (const FieldsPanel::InvalidInput &e)
  {
    wxMessageBox(e.what());
    return;
  }
  Show(false);
  
  Destroy();
  new GameFrame();
}