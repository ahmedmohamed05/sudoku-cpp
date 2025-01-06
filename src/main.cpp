#include "./include/game.h"
#include "./include/timer.h"
#include <iostream>

int main() {
  util::urand();

  Game game;

  Timer timer;
  timer.start();
  while (game.isGameOn()) {
    system("clear");

    game.printGrid();

    game.play();
  }
  timer.stop();

  system("clear");
  game.printGrid();
  std::cout << "Excellent Job You Solved It.\n";
  std::cout << "You Took " << timer.getHours() << ":" << timer.getMinutes()
            << ":" << timer.getSeconds() << " To Solve The Puzzle.\n";

  return 0;
}
