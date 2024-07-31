#include <format>
#include <optional>

#include "serialization.hpp"
#include "settingsdata.hpp"

#include <toml++/toml.hpp>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

const SettingsData settings{"/my_folder/position",
                            std::chrono::milliseconds(100)};
const std::string tomlString{
    "[settings]\ndelay = 100\nposition_path = \"/my_folder/position\""};
const toml::table tomlData = toml::parse(tomlString);

TEST_CASE("Parsing a SettingsData object to TOML")
{
  std::string convertedToml = settingsToToml(settings);

  CHECK(tomlString == convertedToml);
}

TEST_CASE("Parsing a TOML text to a SettingsData")
{
  auto parsedTomlData = toml::parse(tomlString);
  SettingsData expectedSettings =
      SettingsData("", std::chrono::milliseconds(50));
  SettingsData parsedSettings = parseSettings(parsedTomlData);

  INFO("settings.positionPath: " << parsedSettings.positionPath
                                 << "\nsettings.delay: "
                                 << parsedSettings.delay.count())
  CHECK(expectedSettings == parsedSettings);
}

TEST_CASE("Parsing a TOML text with missing fields")
{
  SettingsData defaultSettings{parseSettings("[settings]")};

  bool testSuccesful =
      defaultSettings.positionPath == "" && defaultSettings.delay.count() == 50;

  INFO("parsedSettings.positionPath = " << defaultSettings.positionPath
                                        << "\nparsedSettings.delay = "
                                        << defaultSettings.delay.count())
  CHECK(testSuccesful);
}