#pragma once
#include "./grid.h"
#include <fstream>
#include <iostream>

namespace LOG {
void writeGrid(const Grid::sudokuGrid &grid, std::fstream &file) {

  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      file << grid[i][j].value << " ";

      // Which mean we have print 3 columns
      if ((j + 1) % 3 == 0 && j != 8)
        file << "| ";
    }

    file << '\n';
    // Which mean we have printed 3 rows
    if (((i + 1) % 3 == 0) && i != 8) {
      file << "---------------------\n";
    }
  }
}

} // namespace LOG