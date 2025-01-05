#pragma once
#include "../libs/input-cpp/input.h"
#include "./grid.h"
#include <iostream>

namespace ui {
void printGrid(const sudokuGrid &grid) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      int cell = grid[i][j].value;

      if (cell)
        std::cout << cell;
      else
        std::cout << "·";

      std::cout << " ";

      // Which mean we have print 3 columns
      if ((j + 1) % 3 == 0 && j != 8)
        std::cout << "| ";
    }

    std::cout << '\n';
    // Which mean we have printed 3 rows
    if (((i + 1) % 3 == 0) && i != 8) {
      std::cout << "---------------------\n";
    }
  }
}

Grid::DifficultyLevel readDifficulty() {
  std::cout << "[1]: Easy.\n";
  std::cout << "[2]: Medium.\n";
  std::cout << "[3]: Hard.\n";
  int choice = input::readNumberInRange<int>("Choice Difficulty Level: ", 1, 3);

  return (Grid::DifficultyLevel)choice;
}

} // namespace ui
