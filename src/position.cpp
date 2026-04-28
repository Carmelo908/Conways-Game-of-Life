#include "position.hpp"

#include <algorithm>
#include <fstream>

#include <nlohmann/json.hpp>

std::unique_ptr<Position> openPosition(std::filesystem::path filePath)
{
  std::ifstream jsonFile{filePath};
  const nlohmann::json jsonObject{nlohmann::json::parse(jsonFile)};
  std::unique_ptr<Position> openedPosition;
  auto positionData = jsonObject[0].template get<Position::data_t>();
  openedPosition = std::make_unique<Position>(positionData);
  return openedPosition;
}

Position::Position(data_t &&toCopy)
  : Position(toCopy)
{}

Position::Position(data_t &toCopy)
  : height{static_cast<uint16_t>(toCopy.size())},
    width{static_cast<uint16_t>(toCopy[0].size())},
    data{toCopy},
    cellsQuantity{countCells()},
    genCount{0}
{}

void Position::advanceGen()
{
  const data_t previousGen{data};
  for (uint16_t gameY = 0; gameY < height; gameY++)
  {
    for (uint16_t gameX = 0; gameX < width; gameX++)
    {
      data[gameY][gameX] = updateCell(gameX, gameY, previousGen);
    }
  }
  cellsQuantity = countCells();
  genCount++;
}

bool Position::getCellAt(int coordX, int coordY) const
{
  return data.at(coordY).at(coordX);
}

int Position::getCellsQuantity() const { return cellsQuantity; }

size_t Position::getGenCount() const { return genCount; }

bool Position::operator==(const Position &rhs) const
{
  return this->data == rhs.data;
}

int Position::countCells() const
{
  int countedCells = 0;
  for (const row_t &row : data)
  {
    countedCells += std::count(row.cbegin(), row.cend(), 1);
  }
  return countedCells;
}

bool Position::isOutOfBounds(int cellCoord, int maxCoord) const
{
  return (cellCoord < 0 || cellCoord >= maxCoord);
}

bool Position::updateCell(int cellX, int cellY, const data_t &previousGen)
{
  const bool isCellAlive = previousGen[cellY][cellX];
  const uint16_t neighboursCount =
      sorroundingCellsAt(cellX, cellY, previousGen);
  if (isCellAlive)
  {
    return 1 < neighboursCount && neighboursCount < 4;
  } else
  {
    return neighboursCount == 3;
  }
}

int Position::sorroundingCellsAt(int cellX, int cellY,
                                 const data_t &previousGen) const
{
  int neighboursCount = 0;
  for (int adjY = cellY - 1; adjY < cellY + 2; adjY++)
  {
    if (isOutOfBounds(adjY, height))
    {
      continue;
    }
    for (int adjX = cellX - 1; adjX < cellX + 2; adjX++)
    {
      if (isOutOfBounds(adjX, width))
      {
        continue;
      }
      neighboursCount += previousGen[adjY][adjX];
    }
  }
  return neighboursCount - previousGen[cellY][cellX];
}