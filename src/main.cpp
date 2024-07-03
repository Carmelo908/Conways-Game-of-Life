#include <gui/configmenu.hpp>

class MyApp : public wxApp 
{
public:
  virtual bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
  new ConfigMenu();

  return true;
}
