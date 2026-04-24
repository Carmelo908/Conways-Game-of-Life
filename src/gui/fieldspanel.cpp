#include "fieldspanel.hpp"

#include <filesystem>
#include <stdexcept>

constexpr int gridRows = 2;
constexpr int gridColumns = 2;

FieldsPanel::FieldsPanel(wxFrame *parent, Settings &initialSettings)
  : wxPanel(parent),
    pathInput{new wxFilePickerCtrl()},
    delayInput{new wxSpinCtrl()}
{
  createControls(initialSettings);
  setUpLayout();
}

void FieldsPanel::createControls(Settings &initialSettings)
{
  pathInput->Create(this, wxID_ANY, "", "Select the initial position's file",
                    "*.json");
  pathInput->SetFont(pathInput->GetFont().Scale(1.1));
  pathInput->SetInitialSize(wxSize(200, 30));
  pathInput->SetInitialDirectory("./positions");
  pathInput->SetPath(initialSettings.positionPath.string());
  delayInput->Create(this, wxID_ANY);
  delayInput->SetValue(initialSettings.delay.count());
  delayInput->SetFont(delayInput->GetFont().Scale(1.1));
  delayInput->SetRange(0, 200);
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

  gridSizer->Add(createLabel("Initial position file"));
  gridSizer->Add(createLabel("Delay between generations\n(in milliseconds):"));
  gridSizer->Add(pathInput);
  gridSizer->Add(delayInput);

  SetSizerAndFit(gridSizer);
}

Settings FieldsPanel::getSettingsInput() const
{
  Settings settings;

  settings.positionPath = getPosPath();
  settings.delay = getDelay();

  return settings;
}

std::filesystem::path FieldsPanel::getPosPath() const
{
  return pathInput->GetFileName().GetFullPath().ToStdString();
}

std::chrono::milliseconds FieldsPanel::getDelay() const
{
  return std::chrono::milliseconds(delayInput->GetValue());
}