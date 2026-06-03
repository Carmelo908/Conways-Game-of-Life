#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include <settings.hpp>

TEST_CASE("Testing settings initialization and JSON parsing")
{
  SECTION("Initialization with bad arguments")
  {
    const Settings s1{"", std::chrono::milliseconds(-10), 100};
    const Settings s2{};
    CHECK(s1 == s2);
  }
  SECTION("JSON parsing from invalid data")
  {
    const nlohmann::json invalidJson = {{"position_path", "&&&"},
                                        {"delay", -1}};
    auto result = parseSettings(invalidJson);
    INFO(result.getPositionPath());
    INFO(result.getDelay().count());
    CHECK(result == Settings());
  }
  SECTION("JSON parsing from empty data")
  {
    const nlohmann::json emptyJson{};
    auto result = parseSettings(emptyJson);
    CHECK(result == Settings());
  }
}