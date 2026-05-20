#include "position.hpp"

#include <nlohmann/json.hpp>

Position::Position(CellSet &cellSet)
  : aliveCells{cellSet},
    genCount{}
{}

Position::Position(CellSet &&cellSet)
  : aliveCells{cellSet},
    genCount{}
{}

bool Position::operator==(const Position &rhs) const
{
  return this->aliveCells == rhs.aliveCells;
}

Position::CellSet Position::parseJsonFile(const std::filesystem::path &filepath)
{
  std::ifstream file{filepath};
  return parseJsonFile(file);
}

Position::CellSet Position::parseJsonFile(std::ifstream &file)
{
  auto jsonObject = nlohmann::json::parse(file);
  return parseJson(jsonObject);
}

Position::CellSet Position::parseJson(const nlohmann::json &jsonObject)
{
  CellSet cellSet{};
  for (auto &cell : jsonObject)
  {
    cellSet.insert(CellCoords(cell.at(0), cell.at(1)));
  };
  return cellSet;
}

void Position::advanceGen()
{
  CellSet previousGen;
  std::swap(aliveCells, previousGen);
  aliveCells.reserve(aliveCells.size() * 1.5);
  auto updateCell = [this](CellRef adjacentCell,
                           CellSetRef previousGen) -> void {
    if (determineCell(adjacentCell, previousGen))
    {
      aliveCells.insert(adjacentCell);
    }
  };
  for (CellRef cell : previousGen)
  {
    doOnSorrondingCells(cell, previousGen, updateCell);
  }
  genCount++;
}

bool Position::getCellAt(const CellCoords &cell) const
{
  return aliveCells.contains(cell);
}

bool Position::getCellAt(int64_t coordX, int64_t coordY) const
{
  return aliveCells.contains({.x = coordX, .y = coordY});
}

Position::CellSet::const_iterator Position::begin() const
{
  return aliveCells.begin();
}

Position::CellSet::const_iterator Position::end() const
{
  return aliveCells.end();
}

size_t Position::getCellsQuantity() const { return aliveCells.size(); }

size_t Position::getGenCount() const { return genCount; }

size_t Position::CellCoordsHash::operator()(const CellCoords &cell) const
{
  auto h1 = std::hash<int>()(cell.x);
  auto h2 = std::hash<int>()(cell.y);
  return h1 ^ (h2 << 1U);
};

void Position::doOnSorrondingCells(CellRef cell, CellSetRef previousGen,
                                   auto action) const
{
  for (int adjY = cell.y - 1; adjY < cell.y + 2; adjY++)
  {
    for (int adjX = cell.x - 1; adjX < cell.x + 2; adjX++)
    {
      const CellCoords adjacentCell{.x = adjX, .y = adjY};
      action(adjacentCell, previousGen);
    }
  }
}

bool Position::determineCell(CellRef cell, CellSetRef previousGen) const
{
  const bool isCellAlive = previousGen.contains(cell);
  const uint8_t neighboursCount = sorroundingCellsAt(cell, previousGen);
  if (isCellAlive)
  {
    return 1 < neighboursCount && neighboursCount < 4;
  } else
  {
    return neighboursCount == 3;
  }
}

uint8_t Position::sorroundingCellsAt(CellRef cell, CellSetRef previousGen) const
{
  uint8_t cellsCount = 0;
  auto countCells = [&cellsCount](CellRef adjacentCell,
                                  CellSetRef previousGen) mutable -> void {
    cellsCount += previousGen.contains(adjacentCell);
  };
  doOnSorrondingCells(cell, previousGen, countCells);
  return cellsCount - previousGen.contains(cell);
}