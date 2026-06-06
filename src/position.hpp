#pragma once

#include <filesystem>
#include <fstream>
#include <optional>
#include <unordered_set>

#include <nlohmann/json_fwd.hpp>

/// @brief Represents the coordinates of a cell.
struct CellCoords
{
  int64_t x;
  int64_t y;

  /// @brief default comparision operator.
  bool operator==(const CellCoords &) const = default;
};

/// @brief Represents a Position in the Conway's Game of Life.
class Position
{
  struct CellCoordsHash // Needed for the unordered_set of CellCoords
  {
    size_t operator()(const CellCoords &cell) const;
  };

public:
  /// @brief Type alias for a set of cells with custom hash.
  using CellSet = std::unordered_set<CellCoords, CellCoordsHash>;

  /// @brief Creates a Position from generation 0 copying a CellSet.
  Position(CellSet &cellSet);

  /// @brief Creates a Position from generation 0 moving a CellSet.
  Position(CellSet &&cellSet);

  /// @brief Default copy constructor.
  Position(const Position &) = default;
  /// @brief Default move constructor.
  Position(Position &&) = default;
  /// @brief Default copy assignment.
  Position &operator=(const Position &) = default;
  /// @brief Default move assignment.
  Position &operator=(Position &&) = default;
  /// @brief Default destructor.
  ~Position() = default;

  /// @brief Compares 2 Positions' cells.
  /// @return wether the two Positions have the same cells. Doesn't
  /// compare other properties
  bool operator==(const Position &rhs) const;

  /// @return Empty object if the parsing of the argument file fails or file is
  /// empty, otherwise the opened Position.
  static std::optional<CellSet> parseJson(const std::filesystem::path &file);

  /// @return Empty object if the parsing fails, otherwise the openned Position
  static std::optional<CellSet> parseJson(std::ifstream &file);

  /// \overload
  static std::optional<CellSet> parseJson(const nlohmann::json &jsonObject);

  /// @brief Advances the Position's generation following the Conway's Game of
  /// Life rules.
  void advanceGen();

  /// @return Wether the cell is or not alive in the Position
  bool getCellAt(const CellCoords &cell) const;
  /// \overload
  bool getCellAt(int64_t coordX, int64_t coordY) const;

  /// @return a const iterator to the beggining of the underlying set.
  CellSet::const_iterator begin() const;
  /// @return a const iterator to the end of the underlying set.
  CellSet::const_iterator end() const;

  /// @return how much alive cells there are in the Position.
  size_t getCellsQuantity() const;
  /// @return how much times has advanceGen() been executed.
  size_t getGenCount() const;

private:
  using CellRef = const CellCoords &;
  using CellSetRef = const CellSet &;

  auto updateCellLambda(CellSet &checkedCells);

  void doOnSorrondingCells(CellRef cell, CellSetRef previousGen,
                           auto action) const;

  bool determineCell(CellRef cell, CellSetRef previousGen) const;

  uint8_t sorroundingCellsAt(CellRef cell, CellSetRef previousGen) const;

  CellSet aliveCells;
  size_t genCount;
};