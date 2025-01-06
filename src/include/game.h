#pragma once
#include "../libs/input-cpp/input.h"
#include "../libs/utils/util.h"
#include "./grid.h"
#include <array>
#include <cstdlib>
#include <iostream>

class Game {
private:
  Grid::DifficultyLevel _gameLevel;
  Grid _grid;

  Grid::DifficultyLevel _readDifficulty() {
    std::cout << "[1]: Easy.\n";
    std::cout << "[2]: Medium.\n";
    std::cout << "[3]: Hard.\n";
    int choice =
        input::readNumberInRange<int>("Choice Difficulty Level: ", 1, 3);

    return (Grid::DifficultyLevel)choice;
  }

  Grid::UserGridItem _evaluateInput(std::string input) {

    std::vector<std::string> vInput;

    // Split The Input To Vector
    {
      std::size_t delimPosition;

      while ((delimPosition = input.find(" ")) != input.npos) {
        vInput.push_back(input.substr(0, delimPosition));
        input.erase(0, delimPosition + 1);
      }

      // Pushing last word in string
      if (input != "")
        vInput.push_back(input);

      if (vInput.size() == 0) {
        std::cerr << "Error While Evaluating Input.\n";
        exit(EXIT_FAILURE);
      }
    }

    std::string firstItem = vInput.at(0);
    std::size_t size = vInput.size();

    // Undo/Redo
    if (size == 1) {
      if (firstItem == "u")
        return {-1, -1, -1, Grid::Undo};

      if (firstItem == "r")
        return {-1, -1, -1, Grid::Redo};

      return {-1, -1, -1, Grid::Invalid};
    }

    int row = std::stoi(firstItem);
    int col = std::stoi(vInput.at(1));

    if (!input::isNumberBetween(row, 1, 9))
      return {-1, -1, -1, Grid::Invalid};

    if (!input::isNumberBetween(col, 1, 9))
      return {-1, -1, -1, Grid::Invalid};

    // Removeing Item
    if (size == 2) {
      // Starting from zero
      return {row - 1, col - 1, -1, Grid::Remove};
    }

    // Otherwise It's to insert
    int value = std::stoi(vInput.at(2));

    if (input::isNumberBetween(value, 1, 9)) {
      // Starting from zero
      return {row - 1, col - 1, value, Grid::Add};
    }

    return {-1, -1, -1, Grid::Invalid};
  }

  Grid::UserGridItem _readMove() {

    Grid::UserGridItem item;

    std::cout << "[row] [col] [value] For inserting.\n";
    std::cout << "[row] [col] For removeing.\n";
    std::cout << "Enter [u] For Undo.\n";
    std::cout << "Enter [r] For Redo.\n";

    std::string userInput = input::readString("Enter Your Action: ");
    item = _evaluateInput(userInput);

    return item;
  }

public:
  Game() {
    _gameLevel = _readDifficulty();
    _grid.generateNewGrid(_gameLevel);
  }

  bool isGameOn() const { return !_grid.isSolved(); }

  void printGrid() {
    // Column Indicator
    std::cout << "   1 2 3   4 5 6   7 8 9\n";

    for (int i = 0; i < 9; i++) {
      // Rows Indicator
      std::cout << i + 1 << "  ";

      for (int j = 0; j < 9; j++) {
        int cell = _grid.getGrid()[i][j].value;

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
        // Space in the begging because of the indicator
        std::cout << "   ---------------------\n";
      }
    }
  }

  void play() {
    Grid::UserGridItem move = _readMove();
    bool playedSuccessfully = _grid.play(move);

    while (!playedSuccessfully) {
      std::cerr << "Error Happened maybe for this reasons: \n";
      std::cerr << "Make sure that row and col are between 1 and 9\n";
      std::cerr << "Make Sure that value entered is between 1 and 9.\n";
      std::cerr << "You can't remove a pre-filled cell.\n";
      std::cerr << "Enter [u] or [r] only.\n";
      std::cout << "Press Enter to continue.\n";

      std::cin.get();

      system("clear");
      printGrid();
      move = _readMove();
      playedSuccessfully = _grid.play(move);
    }
  }
};
