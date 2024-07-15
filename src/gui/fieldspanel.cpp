#include <filesystem>
#include <stdexcept>

#include "fieldspanel.hpp"

constexpr int gridRows = 2;
constexpr int gridColumns = 2;

FieldsPanel::FieldsPanel(wxFrame *parent)
  : wxPanel(parent),
    pathInput{new wxFilePickerCtrl()},
    delayInput{new wxSpinCtrl()}
{
  createControls();
  setUpLayout();
}

void FieldsPanel::createControls()
{
  pathInput->Create(this, wxID_ANY, "", "Select the initial position's file",
                    "*.json");
  pathInput->SetFont(pathInput->GetFont().Scale(1.1));
  pathInput->SetInitialSize(wxSize(200, 30));

  delayInput->Create(this, wxID_ANY, "50");
  delayInput->SetFont(delayInput->GetFont().Scale(1.1));
  delayInput->SetRange(1, 10000);
  delayInput->SetInitialSize(wxSize(200, 30));
}

wxStaticText *FieldsPanel::createLabel(std::string_view labelText)
{
  wxStaticText *label = new wxStaticText(this, wxID_ANY, labelText.data());
  label->SetFont(label->GetFont().Scale(1.1));
  return label;
}

void FieldsPanel::setUpLayout()
{
  wxGridSizer *gridSizer = new wxGridSizer(gridRows, gridColumns, 10, 100);

  gridSizer->Add(createLabel("Initial position's file"));
  gridSizer->Add(createLabel("Delay between generations\n(in milliseconds):"));
  gridSizer->Add(pathInput);
  gridSizer->Add(delayInput);

  SetSizerAndFit(gridSizer);
}

SettingsData FieldsPanel::getSettingsInput() const
{
  SettingsData settings;

  settings.position = getPosPath();
  settings.delay = getDelay();

  return settings;
}

std::string FieldsPanel::getPosPath() const
{
  return pathInput->GetFileName().GetFullPath().ToStdString();
}

std::chrono::milliseconds FieldsPanel::getDelay() const
{
  return std::chrono::milliseconds(delayInput->GetValue());
}