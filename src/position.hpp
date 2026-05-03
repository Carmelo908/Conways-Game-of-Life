#pragma once

#include <filesystem>
#include <fstream>
#include <unordered_set>
#include <vector>

#include <nlohmann/json_fwd.hpp>

struct CellCoords
{
  int x, y;

  bool operator==(const CellCoords &) const = default;
};

class Position
{
  struct CellCoordsHash;

public:
  using Row = std::vector<char>;
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
  struct CellCoordsHash
  {
    size_t operator()(const CellCoords cell) const;
  };

  int countCells() const;

  bool insideBounds(int cellCoord, int maxCoord) const;

  void doOnSorrondingCells(CellCoords cell,
                           const Position::CellSet &previousGen,
                           auto action) const;

  bool determineCell(CellCoords cell, const CellSet &previousGen);

  int sorroundingCellsAt(CellCoords cell, const CellSet &previousGen) const;

  CellSet data;
  size_t genCount;
};