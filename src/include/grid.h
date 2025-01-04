#pragma once
#include <array>
#include <iostream>

using sudokuGrid = std::array<std::array<int, 9>, 9>;

class Grid {
private:
  sudokuGrid _grid = {0};

public:
  Grid() {
    for (auto &row : _grid) {
      row.fill(0);
    }
  }

  const sudokuGrid &getGrid() const { return _grid; }
  bool setGrid(sudokuGrid newGrid) {
    // TODO: run some checks here
    _grid = newGrid;
    return true;
  }
};