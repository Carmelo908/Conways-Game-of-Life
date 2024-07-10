#include <vector>
#include <cinttypes>

#pragma once

class Position
{
public:
using row_t = std::vector<uint8_t>;
using data_t = std::vector<row_t>;
  Position(const Position &) = delete;

  Position(data_t toCopy);

  void advanceGen(); // TO DO: refactor this method

  bool getCellAt(const uint16_t coordX, const uint16_t coordY) const;

  uint32_t getCellsQuantity() const;

  const uint16_t height, width;

private:
  void countCells();

  bool isOutOfBounds(int16_t cellCoord, uint16_t maxCoord) const;

  data_t _data;

  uint32_t cellsQuantity;
};