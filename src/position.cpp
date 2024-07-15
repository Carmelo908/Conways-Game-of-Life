#include <algorithm>
#include <utility>
#include <span>

#include "position.hpp"

Position::Position(data_t &&toCopy) :
  _data {toCopy},
  height {static_cast<uint16_t>(toCopy.size())},
  width {static_cast<uint16_t>(toCopy[0].size())},
  cellsQuantity {countCells()},
  genCount {0}
{}

void Position::advanceGen()
{
  const data_t previousGen {_data};

  for (uint16_t gameY = 0; gameY < height; gameY++)
  {
    for (uint16_t gameX = 0; gameX < width; gameX++)
    {
      _data[gameY][gameX] = updateCell(gameX, gameY, previousGen);
    }
  }

  cellsQuantity = countCells();
  genCount++;
}

bool Position::getCellAt(uint16_t coordX, uint16_t coordY) const
{
  return static_cast<bool>(_data.at(coordY).at(coordX));
}

uint32_t Position::getCellsQuantity() const
{
  return cellsQuantity; 
}

size_t Position::getGenCount() const
{
  return genCount;
}

uint32_t Position::countCells()
{
  uint32_t countedCells = 0;
  for (const row_t &row : _data)
  {
    countedCells += std::count(row.cbegin(), row.cend(), 1);
  }
  return countedCells;
}

bool Position::isOutOfBounds(int16_t cellCoord, uint16_t maxCoord) const
{
  return (cellCoord < 0 || cellCoord >= maxCoord);
}

bool Position::updateCell(uint16_t cellX, uint16_t cellY, data_t const &previousGen)
{
  const bool isCellAlive = previousGen[cellY][cellX];
  const uint16_t neighboursCount = 
  sorroundingCellsAt(cellX, cellY, previousGen);

  if (isCellAlive)
  {
    return 1 < neighboursCount && neighboursCount < 4;
  } 
  else
  {
    return neighboursCount == 3;
  }
};

uint8_t Position::sorroundingCellsAt(
  int16_t cellX, int16_t cellY, data_t const &previousGen) const
{
  uint8_t neighboursCount = 0;
  for (int16_t adjY = cellY - 1; adjY < cellY + 2; adjY++)
  {
    if (isOutOfBounds(adjY, height))
    {
      continue;
    }
    for (int16_t adjX = cellX - 1; adjX < cellX + 2; adjX++)
    {
      if (isOutOfBounds(adjX, width))
      {
        continue;     
      }
      neighboursCount += previousGen[adjY][adjX];
    }
  }
  return neighboursCount - previousGen[cellY][cellX];
};