#include "./include/game.h"
#include <iostream>

int main() {
  util::urand();

  Grid grid;

  sudokuGrid newGrid = Generator::generateNewGrid(Generator::easy);

  grid.setGrid(newGrid);

  ui::printGrid(grid.getGrid());

  return 0;
}