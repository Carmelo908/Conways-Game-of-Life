#pragma once

#include <cinttypes>
#include <filesystem>
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <unordered_set>
#include <utility>
#include <vector>

struct CellCoords
{
  int x, y;

  bool operator==(const CellCoords &) const = default;
};

struct CellCoordsHash
{
  size_t operator()(const CellCoords &cell) const
  {
    auto h1 = std::hash<int>()(cell.x);
    auto h2 = std::hash<int>()(cell.y);
    return h1 ^ (h2 << 1);
  }
};

class Position
{
public:
  using Row = std::vector<uint8_t>;
  using Inputdata = std::vector<Row>;
  using CellSet = std::unordered_set<CellCoords, CellCoordsHash>;

  Position(Inputdata &toCopy);
  Position(Inputdata &&toCopy);
  // TODO: change the way input is created so that it matches the
  // new implementation using set of coordinates instead of 2D vector

  static Position parseJsonFile(const std::filesystem::path &file);
  static Position parseJsonFile(std::ifstream &file);
  static Position parseJson(const nlohmann::json &jsonObject);

  void advanceGen();

  bool getCellAt(CellCoords c) const;
  bool getCellAt(int coordX, int coordY) const;

  int getCellsQuantity() const;

  size_t getGenCount() const;

  bool operator==(const Position &rhs) const;

  const int height, width;

private:
  int countCells() const;

  bool isOutOfBounds(int cellCoord, int maxCoord) const;

  bool updateCell(CellCoords c, const CellSet &previousGen);

  int sorroundingCellsAt(CellCoords c, const CellSet &previousGen) const;

  CellSet data;
  size_t genCount;
};