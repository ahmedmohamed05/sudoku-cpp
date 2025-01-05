#include "./include/game.h"
#include "./include/timer.h"
#include <iostream>

int main() {
  util::urand();

  Grid::DifficultyLevel gameLevel = ui::readDifficulty();

  Grid grid(gameLevel);

  while (!grid.isSolved()) {
    system("clear");
    ui::printGrid(grid.getGrid());

    Grid::UserGridItem move = ui::readMove();

    bool playedSuccessfully = grid.play(move);
    while (!playedSuccessfully) {
      std::cerr << "Error Happened maybe for this reasons: \n";
      std::cerr << "Make sure that row and col are between 1 and 9\n";
      std::cerr << "Make Sure that value entered is between 1 and 9.\n";
      std::cerr << "Enter [u] or [r] only.\n";
      std::cout << "Press Enter to continue.\n";

      std::cin.get();
      std::cin.get();

      system("clear");
      ui::printGrid(grid.getGrid());
      move = ui::readMove();
      playedSuccessfully = grid.play(move);
    }
  }

  return 0;
}