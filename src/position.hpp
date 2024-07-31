#include <cinttypes>
#include <vector>

#pragma once

class Position
{
public:
  using row_t = std::vector<uint8_t>;
  using data_t = std::vector<row_t>;

  Position(data_t &&toCopy);

  Position(const Position &) = delete;

  void advanceGen();

  bool getCellAt(uint16_t coordX, uint16_t coordY) const;

  uint32_t getCellsQuantity() const;

  size_t getGenCount() const;

  const uint16_t height, width;

private:
  uint32_t countCells() const;

  bool isOutOfBounds(int16_t cellCoord, uint16_t maxCoord) const;

  bool updateCell(uint16_t cellX, uint16_t cellY, const data_t &previousGen);

  uint8_t sorroundingCellsAt(int16_t cellX, int16_t cellY,
                             const data_t &previousGen) const;

  data_t _data;

  uint32_t cellsQuantity;

  size_t genCount;
};