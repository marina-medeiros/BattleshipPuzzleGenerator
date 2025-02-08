#ifndef _PBG_H_
#define _PBG_H_

#include <list> // std::list
#include <ostream>
#include <set>
#include <vector> // std::vector

#include "common.h"

namespace bpg {

enum class cell_t : unsigned short {
  water = 0,
  battleship,
  destroyer,
  cruiser,
  submarine,
  undefined
};

/// A Cell is a simple struct that represents a location within a puzzle board.
struct Cell {
  using coord_type = short; //!< Alias for a cell coordinage type.

  coord_type row{0}; //!< Row of a cell within a puzzle board.
  coord_type col{0}; //!< Column of a cell within a puzzle board.
  

  //=== Special members
  /// Default constructor
  Cell(coord_type r = 0, coord_type c = 0) : row{r}, col{c} { /*empty*/ }
  /// Destructor
  ~Cell() = default;

  //=== Regular methods
  /// Check if two cells have the same coordinates.
  bool operator==(const Cell &rhs) const {
    return this->row == rhs.row and this->col == rhs.col;
  }
  bool operator!=(const Cell &rhs) const {
    return this->row != rhs.row or this->col != rhs.col;
  }
  Cell& operator=(const Cell& other) = default;
};

/// This class represents a ship.
class Ship {
public:
  cell_t shipType{cell_t::undefined};
  /// Defines the ship orientation.
  enum class orientation : char {
    H = 'H' /* horizontal */,
    V = 'V' /* vertical */,
    U = 'U' /* undefined */
  };
  
  //=== Members
  Cell shipHeadCell;
  orientation shipOrientation;
  int shipSize;
  bool shipPlaced = true;
  bool shipChange_or = true;

  //=== Special Methods:

  /// Default constructor
  Ship(cell_t t = cell_t::battleship,
       Cell hc = Cell(),
       orientation o = orientation::H) 
    : shipType(t), shipHeadCell(hc), shipOrientation(o) {
      if (shipType == cell_t::battleship) {
        shipSize = 4;
      } else if (shipType == cell_t::destroyer) {
        shipSize = 3;
      } else if (shipType == cell_t::cruiser) {
        shipSize = 2;
      } else if (shipType == cell_t::submarine) {
        shipSize = 1;
        shipOrientation = orientation::U;
      } else {
        shipSize = 0;
        shipOrientation = orientation::U;
      }
    }

  /// Destructor.
  ~Ship() = default;

  /// Assignment operator.
  Ship& operator=(const Ship& other) = default;

  //=== Regular methods
  /// Equal operator
  bool operator==(const Ship& rhs) const {
      return (shipType == rhs.shipType && shipOrientation == rhs.shipOrientation && shipHeadCell == rhs.shipHeadCell);
  }


}; // namespace bpg

class Puzzle {
public:
  //=== Members
  short puzzleCols = 10;
  short puzzleRows = 10;
  std::vector<std::vector<cell_t>> puzzleBoard;
  std::vector<Ship> puzzleShips;
  std::string puzzleKey;
  std::string puzzleArmada;
  Cell endLocation = { puzzleRows, 0 };

  /// Default constructor
  Puzzle(int c = 10, int r = 10, Ship s = {}, std::vector<std::vector<cell_t>> b = {}, std::string k = "")
    : puzzleCols(c), puzzleRows(r), puzzleBoard(r, std::vector(c, cell_t::water)), puzzleKey(k) {
    puzzleShips = {
      Ship { cell_t::battleship, Cell(0, 0), Ship::orientation::H },
      Ship { cell_t::destroyer, Cell(0, 0), Ship::orientation::H },
      Ship { cell_t::destroyer, Cell(0, 0), Ship::orientation::H },
      Ship { cell_t::cruiser, Cell(0, 0), Ship::orientation::H },
      Ship { cell_t::cruiser, Cell(0, 0), Ship::orientation::H },
      Ship { cell_t::cruiser, Cell(0, 0), Ship::orientation::H },
      Ship { cell_t::submarine, Cell(0, 0), Ship::orientation::U },
      Ship { cell_t::submarine, Cell(0, 0), Ship::orientation::U },
      Ship { cell_t::submarine, Cell(0, 0), Ship::orientation::U },
      Ship { cell_t::submarine, Cell(0, 0), Ship::orientation::U }
    };
  }


  //=== Regular methods
  void clear();
  static char cellToChar(const cell_t& cell_value);
  bool isLocationWater(const Cell& location) const;
  bool isInsideBoard(const Cell& location);
  std::list<Cell> getShipShadow(const Ship& ship);
  std::list<Cell> getShipBody(const Ship& ship);
  bool addShip(const Ship& ship);
  bool removeShip(const Ship& ship);
};

class Generator {
public:
  static void generatePuzzleKey(Puzzle &pz);
  static void generateAux(int index, Puzzle &pz, std::set<std::string> &pzKeys, const RunningOpt &opt, std::list<Puzzle> &puzzleList);
  [[nodiscard]] static std::list<Puzzle> generate(const RunningOpt &opt);
  static void generateArmada(Puzzle &pz);
  static Cell nextLocation(const Cell& current, Puzzle &pz);
};

}
#endif
