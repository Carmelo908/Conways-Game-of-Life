#include <fstream>
#include <cinttypes>
#include <vector>
#include <cinttypes>
#include <memory>

#include <gui.hpp>
#include <position.hpp>

class MyApp : public wxApp 
{
public:
  virtual bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
  MainFrame *mainFrame = new MainFrame();

  return true;
}
