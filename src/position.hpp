#include <vector>
#include <cinttypes>
#include <memory>

#pragma once

class Position
{
public:
  using row_t = std::vector<uint8_t>;
  using data_t = std::vector<row_t>;

  Position(data_t toCopy) :
    height {static_cast<uint16_t>(toCopy.size())},
    width {static_cast<uint16_t>(toCopy[0].size())},
    _data {toCopy},
    cellsQuantity {}
  {
    countCells();
  };

  void advanceGen() // TO DO: refactor this method
  {
    const data_t previousGen {_data};

    for (uint16_t posY = 0; posY < height; posY++)
    {
      for (uint16_t posX = 0; posX < width; posX++)
      {
        if (!previousGen[posY][posX]) 
        continue;

        uint8_t sorroundingCells = 0;

        for (int16_t cellY = -1; cellY < 2; cellY++)
        {
          if (isOutOfBounds(posY + cellY, height))
          continue;

          for (int16_t cellX = -1; cellX < 2; cellX++)
          {
            if (isOutOfBounds(posX + cellX, width)) 
            continue;

            sorroundingCells += previousGen[posY + cellY][posX + cellX];

            if (previousGen[posY + cellY][posX + cellX])
            continue;

            uint8_t sorroundingDeadCell = 0;
            for (int8_t deadCellY = -1; deadCellY < 2; deadCellY++)
            {
              if (isOutOfBounds(posY + cellY + deadCellY, height)) 
              continue;

              for (int8_t deadCellX = -1; deadCellX < 2; deadCellX++)
              {
                if (isOutOfBounds(posX + cellX + deadCellX, width))
                continue;

                sorroundingDeadCell += 
                previousGen[posY + cellY + deadCellY][posX + cellX + deadCellX];
              }
            }

            _data[posY + cellY][posX + cellX] = (sorroundingDeadCell == 3);
          }
        }

        _data[posY][posX] = (sorroundingCells == 4 || sorroundingCells == 3);
      }
    }

    countCells();
  }

  const uint16_t height, width;

  const bool getCellAt(uint16_t coordX, uint16_t coordY)
  {
    return static_cast<bool>(_data.at(coordY).at(coordX));
  };

  const uint getCellsQuantity() { return cellsQuantity; }

private:
  void countCells()
  {
    uint countedCells = 0;
    for (const row_t &row : _data)
    {
      countedCells += std::count(row.begin(), row.end(), 1);
    }
    cellsQuantity = countedCells;
  }

  const bool isOutOfBounds(const uint16_t cellCoord, const uint16_t maxCoord)
  {
    return (cellCoord < 0 || cellCoord >= maxCoord);
  }

  data_t _data;

  uint cellsQuantity;
};
