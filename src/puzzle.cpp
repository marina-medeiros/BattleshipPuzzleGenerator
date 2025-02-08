#include <iostream> // std::cout, std::endl
#include <vector>
#include <algorithm>

#include "include/bpg.h"
#include "include/common.h"

namespace bpg{
  
  /**
 * @brief Clears the puzzle board by setting all cells to water.
 * 
 * This function iterates through each cell of the puzzle board and sets its value to 'water'.
 */
  void Puzzle::clear() {
    for (auto& row : puzzleBoard) {
      for (auto& col : row) {
        col = cell_t::water;
      }
    }
  }

  /**
 * @brief Converts a cell value to its corresponding character representation (water, battleship, destroyer, cruiser, submarine or undefined).
 * 
 * This function maps the given cell value to its corresponding character representation based on a predefined set of labels.
 * 
 * @param cell_value The value of the cell to be converted.
 * @return The character representation of the cell value.
 */
  char Puzzle::cellToChar(const cell_t& cell_value) {
    std::string_view labels{ " BDCSU" };
    return labels[(unsigned short)cell_value];
  }

/**
 * @brief Checks if the given location represents water.
 * 
 * This function checks if the cell at the given location represents water.
 * 
 * @param location The location to check.
 * @return True if the cell contains water, false otherwise.
 */
  bool Puzzle::isLocationWater(const Cell& location) const {
    return puzzleBoard[location.row][location.col] == cell_t::water;
  }

/**
 * @brief Checks if the given location lies inside the board limits.
 * 
 * This function checks if the given location lies within the boundaries of the puzzle board.
 * 
 * @param location The location to check.
 * @return True if the location is inside the board limits, false otherwise.
 */
  bool Puzzle::isInsideBoard(const Cell& location) {
    return location.row >= 0 and location.row < puzzleRows and location.col >= 0
          and location.col < puzzleCols;
  }

/**
 * @brief Gets the shadow cells (body + margins) of a ship.
 * 
 * This function calculates the shadow cells of a ship based on its head cell, orientation, and size.
 * 
 * @param ship The ship for which to calculate the shadow cells.
 * @return A list of cells representing the ship's shadow.
 */
  std::list<Cell> Puzzle::getShipShadow(const Ship& ship) {
    std::list<Cell> cells;
    short limit_row = (ship.shipOrientation == Ship::orientation::H) ? 1 : ship.shipSize;
    short limit_col = (ship.shipOrientation == Ship::orientation::V) ? 1 : ship.shipSize;
    for (short offset_row = -1; offset_row <= limit_row; offset_row++) {
      for (short offset_col = -1; offset_col <= limit_col; offset_col++) {
        Cell ship_shadow(ship.shipHeadCell.row + offset_row, ship.shipHeadCell.col + offset_col);
        if (isInsideBoard(ship_shadow)) {
          cells.emplace_front(ship_shadow);
        }
      }
    }
    return cells;
  }
  
/**
 * @brief Returns a list of cells representing the locations under the ship's body.
 * 
 * This function calculates and returns a list of cells that are under the body of the given ship. 
 * It iterates through the cells under the ship's body based on its head cell, orientation, and size.
 * 
 * @param ship The ship for which to calculate the body cells.
 * @return A list of cells representing the locations under the ship's body.
 */
  std::list<Cell> Puzzle::getShipBody(const Ship& ship) {
    std::list<Cell> cells;
    short limit_row = (ship.shipOrientation == Ship::orientation::H) ? 1 : ship.shipSize;
    short limit_col = (ship.shipOrientation == Ship::orientation::V) ? 1 : ship.shipSize;
    for (short offset_row = 0; offset_row < limit_row; offset_row++) {
      for (short offset_col = 0; offset_col < limit_col; offset_col++) {
        Cell ship_body(ship.shipHeadCell.row + offset_row, ship.shipHeadCell.col + offset_col);
        if (isInsideBoard(ship_body)) {
          cells.emplace_front(ship_body);
        }
      }
    }
    return cells;
  }

/**
 * @brief Adds a ship to the puzzle board.
 * 
 * This function attempts to add the given ship to the puzzle board.
 * It checks if the ship's shadow cells are empty and if the ship's body cells are all water.
 * If both conditions are met, the ship is added to the puzzle board.
 * 
 * @param ship The ship to be added to the puzzle board.
 * @return True if the ship was successfully added, false otherwise.
 */
  bool Puzzle::addShip(const Ship& ship) {
    auto ship_shadow_cells = getShipShadow(ship);
    if (ship_shadow_cells.empty()) {
      return false;
    }
    if (std::all_of(ship_shadow_cells.cbegin(), ship_shadow_cells.cend(), [&](const Cell& c) {
          return this->isLocationWater(c);
        })) {
      auto ship_body_cells = getShipBody(ship);
      if (ship_body_cells.empty() or int(ship_body_cells.size()) != ship.shipSize
          or not std::all_of(ship_body_cells.cbegin(), ship_body_cells.cend(), [&](const Cell& c) {
              return this->isLocationWater(c);
            })) {
        return false;
      }
      for (const Cell& loc : ship_body_cells) {
        puzzleBoard[loc.row][loc.col] = ship.shipType;
      }
      return true;
    }
    return false;
  }


/**
 * @brief Removes a ship from the puzzle board.
 * 
 * This function removes the given ship from the puzzle board by setting all its shadow cells to water.
 * 
 * @param ship The ship to be removed from the puzzle board.
 * @return True if the ship was successfully removed, false otherwise.
 */
  bool Puzzle::removeShip(const Ship& ship) {
    auto ship_shadow_cells = getShipShadow(ship);
    if (ship_shadow_cells.empty()) {
      return false;
    }
    for (const Cell& loc : ship_shadow_cells) {
      puzzleBoard[loc.row][loc.col] = cell_t::water;
    }
    return true;
  }

};