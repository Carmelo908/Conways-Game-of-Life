#include <wx/wx.h>

#include "fieldspanel.hpp"
#include "../settingsdata.hpp"

#pragma once

class ConfigMenu : public wxFrame
{
public:
  ConfigMenu();

private:
  void createButton();

  void setUpLayout();

  void OnAcceptButton(wxCommandEvent &);
  
  wxButton *acceptButton;
  FieldsPanel *fieldsPanel;
};