#define CATCH_CONFIG_MAIN
#include <filesystem>
#include <string>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

#include <position.hpp>

auto benchmarkFilename = "benchmark_position.json";

std::string posToString(Position &pos)
{
  std::string posString = "";
  for (int y = 0; y < pos.height; y++)
  {
    for (int x = 0; x < pos.width; x++)
    {
      posString += pos.getCellAt(x, y) ? " #" : " -";
    }
    posString += "\n";
  }
  return posString;
}

TEST_CASE("Opening a position from json")
{
  auto json = nlohmann::json::parse(R"([
    [0, 1, 0],
    [1, 1, 1],
    [0, 1, 0]
  ])");
  Position pos = Position::parseJson(json);
  Position expected{{
      {0, 1, 0},
      {1, 1, 1},
      {0, 1, 0},
  }};
  REQUIRE(pos == expected);
}

TEST_CASE("Position functionality")
{
  SECTION("Comparision operator")
  {
    Position::Inputdata data{{{0, 1}, {1, 0}}};
    Position pos1{data};
    Position pos2{data};

    CHECK(pos1 == pos2);
  }

  SECTION("Advance generation", "[!benchmark]")
  {
    Position pos{{
        {0, 1, 0},
        {1, 1, 0},
        {0, 0, 1},
    }};
    pos.advanceGen();
    Position nextPos{{
        {1, 1, 0},
        {1, 1, 1},
        {0, 1, 0},
    }};
    INFO(posToString(pos));
    REQUIRE(pos == nextPos);
    BENCHMARK_ADVANCED("Advance 50x50 generation 100 times")
    (Catch::Benchmark::Chronometer meter)
    {
      auto benchmarkFilepath =
          std::filesystem::path(__FILE__).remove_filename().append(
              benchmarkFilename);
      Position benchmarkPosition{Position::parseJsonFile(benchmarkFilepath)};
      auto advance100Times = [&]() {
        for (int i = 0; i < 100; i++)
        {
          benchmarkPosition.advanceGen();
        }
      };
      meter.measure([&]() {
        return advance100Times();
      });
    };
  }
}