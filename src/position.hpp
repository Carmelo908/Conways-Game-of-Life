#pragma once

#include <cinttypes>
#include <filesystem>
#include <fstream>
#include <unordered_set>
#include <vector>

#include <nlohmann/json_fwd.hpp>

/// @brief Represents the coordinates of a cell, generally an alive one.
struct CellCoords
{
  int64_t x;
  int64_t y;

  bool operator==(const CellCoords &) const = default;
};

/// @brief Represents a Position in the Conway's Game of Life with fixed
/// dimensions.
class Position
{
  struct CellCoordsHash // Needed for the unordered_set of CellCoords
  {
    size_t operator()(const CellCoords &cell) const;
  };

public:
  /// @brief Type alias for a row in a 2d vector which contains the data to
  /// construct a Position.
  using Row = std::vector<char>;

  /// @brief Type alias for the data that the constructors recieve to create the
  /// Position. A patch to match the constructions with the current way cells
  /// are stores is pending.
  using Inputdata = std::vector<Row>;

  /// @brief Type alias for a set of cells.
  using CellSet = std::unordered_set<CellCoords, CellCoordsHash>;

  /// @brief Creates a Position from generation 0 with a Inputdata. Sets the
  /// width and height depending on the input size
  Position(Inputdata &toCopy);

  /// @brief Move constructor. Delegates to Position(InputData &)
  Position(Inputdata &&toCopy);
  // \todo change the way input is created so that it matches the
  // new implementation using set of coordinates instead of 2D vector

  /// @brief Creates a position from a JSON file
  /// @param file: file path expected to be non empty and refer to a json file.
  /// Delegates to parseJsonFile(const std::ifstream &).
  /// @return The opened Position
  static Position parseJsonFile(const std::filesystem::path &file);

  /// @brief Delegates actual parsing of file's contents to parseJson(const
  /// nlohmann::json &).
  /// @return The openned Position
  static Position parseJsonFile(std::ifstream &file);

  /// @brief Open a Position from a nlohmann::json object whose data is expected
  /// to be valid.
  /// @throws nlohmann::exception if the JSON data is invalid.
  /// @return The openned Position.
  static Position parseJson(const nlohmann::json &jsonObject);

  /// @brief Advances the Position's generation following the Conway's Game of
  /// Life rules.
  void advanceGen();

  /// @return Wether the cell is or not alive in the Position
  bool getCellAt(const CellCoords &cell) const;
  /// \overload
  bool getCellAt(int64_t coordX, int64_t coordY) const;

  /// @return an const iterator to the first cell.
  CellSet::const_iterator begin() const;
  /// @return an const iterator to the last cell.
  CellSet::const_iterator end() const;

  /// @return how much alive cells there are in the Position.
  size_t getCellsQuantity() const;
  /// @return how much times has advanceGen() been executed.
  size_t getGenCount() const;
  /// @return the width of the Position determined on creation.
  int getWidth() const;
  /// @return the height of the Position determined on creation.
  int getHeight() const;

  /// @brief Compares 2 Positions
  /// @return wether the two Positions have the same cells. Doesn't
  /// compare other properties
  bool operator==(const Position &rhs) const;

private:
  using CellRef = const CellCoords &;
  using CellSetRef = const CellSet &;

  void doOnSorrondingCells(CellRef cell, CellSetRef previousGen,
                           auto action) const;

  bool determineCell(CellRef cell, CellSetRef previousGen) const;

  uint8_t sorroundingCellsAt(CellRef cell, CellSetRef previousGen) const;

  int height;
  int width;
  CellSet aliveCells;
  size_t genCount;
};