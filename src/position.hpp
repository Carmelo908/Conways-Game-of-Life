#pragma once

#include <cinttypes>
#include <filesystem>
#include <memory>
#include <vector>

class Position;

std::unique_ptr<Position> openPosition(std::filesystem::path filePath);

class Position
{
public:
  using row_t = std::vector<uint8_t>;
  using data_t = std::vector<row_t>;

  Position(data_t &toCopy);
  Position(data_t &&toCopy);

  void advanceGen();

  bool getCellAt(int coordX, int coordY) const;

  int getCellsQuantity() const;

  size_t getGenCount() const;

  bool operator==(const Position &rhs) const;

  const int height, width;

private:
  int countCells() const;

  bool isOutOfBounds(int cellCoord, int maxCoord) const;

  bool updateCell(int cellX, int cellY, const data_t &previousGen);

  int sorroundingCellsAt(int cellX, int cellY, const data_t &previousGen) const;

  data_t data;
  int cellsQuantity;
  size_t genCount;
};