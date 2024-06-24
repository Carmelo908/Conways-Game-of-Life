#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <position.hpp>

#include <iostream>
#include <string>
#include <fstream>

std::string posToString(Position &pos)
{
  std::string posString = "";

  for (size_t y = 0; y < pos.height; y++)
  {
    for (size_t x = 0; x < pos.width; x++)
    {
      posString += pos.getCellAt(x, y) ? " #" : " -";
    }
    posString += "\n";
  }

  return posString;
}

bool compareFiles(std::string logsPath = "./logs.txt", 
  std::string sequencePath = "./rightSequence.txt") 
{
  std::ifstream logFile {std::ifstream(logsPath)};
  std::ifstream sequenceFile {std::ifstream(sequencePath)};

  std::string logLine = "";
  std::string sequenceLine = "";

  while (std::getline(sequenceFile, sequenceLine))
  {
    std::getline(logFile, logLine);
    if (logLine != sequenceLine)
     return false;   
  }

  return true;
}

TEST_CASE("Compute fixed 10X10 position")
{
  std::ofstream logFile {"./logs.txt"};

  Position::data_t posData {10, std::vector<uint8_t>(10)};

  posData = {{
    {1, 1, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 1, 0, 0},
    {1, 0, 1, 1, 1, 1, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
    {1, 1, 1, 0, 1, 0, 0, 0, 0, 0},
    {1, 0, 1, 0, 0, 1, 0, 1, 1, 0},
    {0, 1, 0, 1, 1, 0, 0, 0, 1, 0},
    {0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
    {0, 1, 0, 1, 0, 0, 1, 1, 0, 0},
  }};

  Position currentGen {posData};

  for (size_t i = 0; i < 11; i++)
  {
    std::string posString = posToString(currentGen) +
    "gen: " + std::to_string(i) + "\n" +
    "cells: " + std::to_string(currentGen.getCellsQuantity()) + "\n\n";

    logFile << posString;

    currentGen.advanceGen();
  }
  logFile.close();

  CHECK(compareFiles());
}