#include "position.hpp"

#include <algorithm>
#include <fstream>

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
  const CellSet previousGen{data};
  data.clear();
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      const CellCoords cell{x, y};
      if (updateCell(cell, previousGen))
      {
        data.insert(cell);
      } else
      {
        data.erase(cell);
      }
    }
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

bool Position::isOutOfBounds(int cellCoord, int maxCoord) const
{
  return (cellCoord < 0 || cellCoord >= maxCoord);
}

bool Position::updateCell(CellCoords cell, const CellSet &previousGen)
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
  int neighboursCount = 0;
  for (int adjY = cell.y - 1; adjY < cell.y + 2; adjY++)
  {
    for (int adjX = cell.x - 1; adjX < cell.x + 2; adjX++)
    {
      neighboursCount += previousGen.contains({adjX, adjY});
    }
  }
  return neighboursCount - previousGen.contains(cell);
}