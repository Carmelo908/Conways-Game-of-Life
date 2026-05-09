#pragma once

#include <filesystem>
#include <fstream>
#include <unordered_set>
#include <vector>

#include <nlohmann/json_fwd.hpp>

/// @brief Represents the coordinates of a cell, generally an alive one.
struct CellCoords
{
  int x, y;

  bool operator==(const CellCoords &) const = default;
};

/// @brief Represents a Position in the Conway's Game of Life with fixed
/// dimensions.
class Position
{
  struct CellCoordsHash;

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
  /// @return The openned Position
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

  /// @brief Advances the generation following the Conway's Game of Life rules.
  void advanceGen();

  /// @brief Searchs for a cell in the current Position's state
  /// @param c the cell coordinates
  /// @return Wether the cell is or not alive
  bool getCellAt(CellCoords c) const;
  /// \overload
  bool getCellAt(int coordX, int coordY) const;
  /// @return how much alive cells there are in the Position.
  int getCellsQuantity() const;
  /// @return how much times has advanceGen() been executed.
  size_t getGenCount() const;
  /// @return the width of the Position determined on creation.
  int getWidth() const;
  /// @return the height of the Position determined on creation.
  int getHeight() const;

  /// @brief Compares 2 Positions
  /// @param rhs the other object to compare
  /// @return wether the two Positions have the same alive cells. Doesn't
  /// compare other properties
  bool operator==(const Position &rhs) const;

private:
  struct CellCoordsHash
  {
    size_t operator()(CellCoords cell) const;
  };

  int countCells() const;

  bool insideBounds(int cellCoord, int maxCoord) const;

  void doOnSorrondingCells(CellCoords cell,
                           const Position::CellSet &previousGen,
                           auto action) const;

  bool determineCell(CellCoords cell, const CellSet &previousGen);

  int sorroundingCellsAt(CellCoords cell, const CellSet &previousGen) const;

  int height;
  int width;
  CellSet aliveCells;
  CellSet previousGen;
  // Cache member for better performance, specially in large positions
  size_t genCount;
};