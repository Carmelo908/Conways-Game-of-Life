#include <vector>
#define CATCH_CONFIG_MAIN
#include <filesystem>
#include <string>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include <position.hpp>

const auto positionFilename = "testing_position.json";

using namespace std::string_literals;

// for 3x3 samples only
std::string positionToString(const Position &pos)
{
  std::string posString;
  for (int y = -1; y < 2; y++)
  {
    for (int x = -1; x < 2; x++)
    {
      posString += pos.getCellAt(x, y) ? " #" : " -";
    }
    posString += '\n';
  }
  return posString;
}

Position stringToPosition(std::vector<std::string> &&posStringsSpan)
{
  Position::CellSet cellSet{};
  for (uint y = 0; y < posStringsSpan.size(); y++)
  {
    for (uint x = 0; x < posStringsSpan.at(y).size(); x++)
    {
      if (posStringsSpan.at(y).at(x) == '#')
      {
        cellSet.insert(CellCoords(x, y));
      }
    }
  }
  return {cellSet};
}

TEST_CASE("Opening a position from json")
{
  auto json = nlohmann::json::parse(R"([
    [0, -1], [0, 0], [0, 1]
  ])");
  Position pos{Position::parseJson(json)};
  Position::CellSet expectedCells{};
  expectedCells.insert({.x = 0, .y = -1});
  expectedCells.insert({.x = 0, .y = 0});
  expectedCells.insert({.x = 0, .y = 1});
  Position expected{expectedCells};
  REQUIRE(pos == expected);
}

TEST_CASE("Position functionality")
{
  SECTION("Advance generation")
  {
    Position pos{stringToPosition({
        "-#-"s,
        "##-"s,
        "--#"s,
    })};
    INFO("intial position =\n" << positionToString(pos));
    pos.advanceGen();

    Position nextPos{stringToPosition({
        "##-"s,
        "###"s,
        "-#-"s,
    })};
    INFO("result =\n" << positionToString(pos));
    INFO("expected =\n" << positionToString(nextPos));
    REQUIRE(pos == nextPos);
  };

  BENCHMARK_ADVANCED("Advance fixed Position 100 times")
  (Catch::Benchmark::Chronometer meter)
  {
    auto benchmarkFilepath =
        std::filesystem::path(__FILE__).remove_filename().append(
            positionFilename);
    Position benchmarkPosition{Position::parseJsonFile(benchmarkFilepath)};
    auto advance100Times = [&] {
      for (int i = 0; i < 100; i++)
      {
        benchmarkPosition.advanceGen();
      }
    };
    meter.measure([&] {
      return advance100Times();
    });
  };
}