#include "settingspanel.hpp"

#include <memory>
#include <wx/event.h>
#include <wx/filepicker.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>

#include "../settings.hpp"

constexpr int gridRows = 4;
constexpr int gridColumns = 1;

wxDEFINE_EVENT(EVT_SETTINGS_UPDATED, SettingsUpdateEvent);

SettingsUpdateEvent::SettingsUpdateEvent(std::shared_ptr<Settings> &&settings,
                                         bool positionChanged)
  : wxCommandEvent(EVT_SETTINGS_UPDATED, wxID_ANY),
    settings{std::move(settings)},
    positionChanged{positionChanged}

{}

std::shared_ptr<Settings> SettingsUpdateEvent::getSettings()
{
  return settings;
}

bool SettingsUpdateEvent::hasPositionChanged() const { return positionChanged; }

wxEvent *SettingsUpdateEvent::Clone() const
{
  return new SettingsUpdateEvent(*this);
}

SettingsPanel::SettingsPanel(wxWindow *parent, const Settings &initialSettings)
  : wxPanel(parent),
    pathCtrl{new wxFilePickerCtrl()},
    delayCtrl{new wxSpinCtrl()}
{
  SetFont(GetFont().Scale(1.1));
  createControls(initialSettings);
  setUpLayout();
  Bind(wxEVT_FILEPICKER_CHANGED, &SettingsPanel::onSettingsChanged, this);
  Bind(wxEVT_SPINCTRL, &SettingsPanel::onSettingsChanged, this);
}

void SettingsPanel::createControls(const Settings &initialSettings)
{
  pathCtrl->Create(this, wxID_ANY, "", "Open position file", "*.json");
  pathCtrl->SetInitialSize(wxSize(200, 30));
  pathCtrl->SetInitialDirectory("./positions");
  pathCtrl->SetPath(initialSettings.getPositionPath().string());
  delayCtrl->Create(this, wxID_ANY);
  delayCtrl->SetInitialSize(wxSize(200, 30));
  delayCtrl->SetValue(initialSettings.getDelay().count());
  delayCtrl->SetRange(0, 1000);
}

wxStaticText *SettingsPanel::createLabel(std::string_view labelText)
{
  return new wxStaticText(this, wxID_ANY, labelText.data());
}

void SettingsPanel::setUpLayout()
{
  wxGridSizer *mainSizer = new wxGridSizer(gridRows, gridColumns, 10, 100);
  mainSizer->Add(createLabel("Initial position file"));
  mainSizer->Add(pathCtrl);
  mainSizer->Add(createLabel("Delay between generations\n(in milliseconds):"));
  mainSizer->Add(delayCtrl);
  SetSizerAndFit(mainSizer);
}

Settings SettingsPanel::getSettings() const
{
  Settings settings{};
  auto positionPath = getPositionPath();
  settings.setPositionPath(positionPath);
  settings.setDelay(getDelay());
  return settings;
}

std::filesystem::path SettingsPanel::getPositionPath() const
{
  return pathCtrl->GetFileName().GetFullPath().ToStdString();
}

std::chrono::milliseconds SettingsPanel::getDelay() const
{
  return std::chrono::milliseconds(delayCtrl->GetValue());
}

void SettingsPanel::onSettingsChanged(wxCommandEvent &event)
{
  const bool positionChanged = event.GetEventType() == wxEVT_FILEPICKER_CHANGED;
  SettingsUpdateEvent updateEvent{std::make_shared<Settings>(getSettings()),
                                  positionChanged};
  GetParent()->ProcessWindowEvent(updateEvent);
}