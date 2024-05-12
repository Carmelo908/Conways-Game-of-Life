#include <fstream>
#include <cinttypes>
#include <vector>
#include <cinttypes>
#include <memory>

#include <gui.hpp>
#include <position.hpp>

Position* setupPosition() 
{
  std::ifstream jsonFile {"./positions/random.json"};

  const nlohmann::json jsonObject {nlohmann::json::parse(jsonFile)};

  Position *currentGen {new Position(jsonObject[0].template get<Position::data_t>())};

  return currentGen;
}

class MyApp : public wxApp 
{
public:
  virtual bool OnInit() 
  {
    MainFrame *mainFrame = new MainFrame(setupPosition());
    mainFrame->Show(true);

    return true;
  }
};


wxIMPLEMENT_APP(MyApp);
