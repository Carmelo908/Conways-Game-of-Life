#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include <position.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <utility>

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

TEST_CASE("Testing Position functionality")
{
  SECTION("Comparision operator")
  {
    Position::data_t data{{{0, 1}, {1, 0}}};
    Position pos1{data};
    Position pos2{data};

    bool equal = pos1 == pos2;
    REQUIRE(equal);
  }

  SECTION("Advance generation")
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
    bool equal = pos == nextPos;
    REQUIRE(equal);
  }
}