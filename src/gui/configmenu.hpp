#include <wx/wx.h>

#include "fieldspanel.hpp"
#include "gameframe.hpp"
#include "../settingsdata.hpp"

#pragma once

class ConfigMenu : public wxFrame
{
public:
  ConfigMenu();

private:
  void OnAcceptButton(wxCommandEvent &);
  
  wxButton *acceptButton;
  FieldsPanel *fieldsPanel;
};