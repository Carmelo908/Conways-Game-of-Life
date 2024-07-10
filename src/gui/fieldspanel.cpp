#include <filesystem>
#include <stdexcept>

#include "gui/fieldspanel.hpp"

FieldsPanel::FieldsPanel(wxFrame *parent) :
  wxPanel(parent),
  pathInput {new wxTextCtrl(this, wxID_ANY, "./positions/position.json",
  wxDefaultPosition, wxSize(200, 30))},
  delayInput {new wxSpinCtrl(this, wxID_ANY, "50", 
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

SettingsData FieldsPanel::getSettingsInput() const
{
  SettingsData settings;

  settings.posPath = getPosPath(); 
  settings.delay = getDelay();

  return settings;
}

std::string FieldsPanel::getPosPath() const
{
  std::string posPath = pathInput->GetValue().ToStdString();

  if (!std::filesystem::exists(posPath))
  {
    throw InvalidInput("The file doesn't exist\n");
  }
  else
  {
    return posPath;
  }
}

std::chrono::milliseconds FieldsPanel::getDelay() const
{
  return std::chrono::milliseconds(delayInput->GetValue());
}