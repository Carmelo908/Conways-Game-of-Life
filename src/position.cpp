#include "position.hpp"

#include <algorithm>
#include <fstream>
#include <utility>

size_t CellCoordsHash::operator()(const CellCoords &cell) const
{
  auto h1 = std::hash<int>()(cell.x);
  auto h2 = std::hash<int>()(cell.y);
  return h1 ^ (h2 << 1);
};

Position Position::parseJsonFile(const std::filesystem::path &filepath)
{
  std::ifstream file{filepath};
  return parseJsonFile(file);
}

Position Position::parseJsonFile(std::ifstream &file)
{
  auto jsonObject = nlohmann::json::parse(file);
  return parseJson(jsonObject);
}

Position Position::parseJson(const nlohmann::json &jsonObject)
{
  auto positionData = jsonObject.template get<Position::Inputdata>();
  return Position(positionData);
}

Position::Position(Inputdata &toCopy)
  : height{static_cast<int>(toCopy.size())},
    width{static_cast<int>(toCopy[0].size())},
    data{},
    genCount{0}
{
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      if (toCopy[y][x])
      {
        data.emplace(x, y);
      }
    }
  }
}

Position::Position(Inputdata &&toCopy)
  : Position(toCopy)
{}

void Position::advanceGen()
{
  CellSet previousGen{data};
  previousGen.reserve(data.size() * 1.5);
  for (const CellCoords cell : previousGen)
  {
    auto updateCell = [&](const CellCoords adjacentCell,
                          const CellSet &previousGen) -> void {
      bool cellNextState = determineCell(adjacentCell, previousGen);
      if (cellNextState)
      {
        data.insert(adjacentCell);
      } else
      {
        data.erase(adjacentCell);
      }
    };
    doOnSorrondingCells(cell, previousGen, updateCell);
  }
  genCount++;
}

bool Position::getCellAt(CellCoords c) const { return data.contains(c); }

bool Position::getCellAt(int coordX, int coordY) const
{
  return data.contains({coordX, coordY});
}

int Position::getCellsQuantity() const { return data.size(); }

size_t Position::getGenCount() const { return genCount; }

bool Position::operator==(const Position &rhs) const
{
  return this->data == rhs.data;
}

int Position::countCells() const { return data.size(); }

bool Position::insideBounds(int cellCoord, int maxCoord) const
{
  return (cellCoord >= 0 && cellCoord < maxCoord);
}

void Position::doOnSorrondingCells(CellCoords cell,
                                   const Position::CellSet &previousGen,
                                   auto action) const
{
  for (int adjY = cell.y - 1; adjY < cell.y + 2; adjY++)
  {
    if (!insideBounds(adjY, height))
    {
      continue;
    }
    for (int adjX = cell.x - 1; adjX < cell.x + 2; adjX++)
    {
      if (!insideBounds(adjX, width))
      {
        continue;
      }
      const CellCoords adjacentCell{adjX, adjY};
      action(adjacentCell, previousGen);
    }
  }
}

bool Position::determineCell(CellCoords cell, const CellSet &previousGen)
{
  const bool isCellAlive = previousGen.contains(cell);
  const uint16_t neighboursCount = sorroundingCellsAt(cell, previousGen);
  if (isCellAlive)
  {
    return 1 < neighboursCount && neighboursCount < 4;
  } else
  {
    return neighboursCount == 3;
  }
}

int Position::sorroundingCellsAt(CellCoords cell,
                                 const CellSet &previousGen) const
{
  int cellsCount = 0;
  auto countCells = [this, &cellsCount](const CellCoords adjacentCell,
                                        const CellSet &previousGen) mutable {
    if (previousGen.contains(adjacentCell))
    {
      cellsCount++;
    }
  };
  doOnSorrondingCells(cell, previousGen, countCells);
  return cellsCount - previousGen.contains(cell);
}