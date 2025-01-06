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

  return 0;
}
