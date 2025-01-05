#pragma once
#include "../libs/input-cpp/input.h"
#include "./grid.h"

#include <iostream>
#include <vector>

namespace ui {

Grid::UserGridItem evaluateInput(std::string input) {

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
    return {row, col, -1, Grid::Remove};
  }

  // Otherwise It's to insert
  int value = std::stoi(vInput.at(2));

  if (input::isNumberBetween(value, 1, 9)) {
    return {row, col, value, Grid::Add};
  }

  return {-1, -1, -1, Grid::Invalid};
}

void printGrid(const Grid::sudokuGrid &grid) {
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

Grid::UserGridItem readMove() {

  Grid::UserGridItem item;

  std::cout << "[row] [col] [value] For inserting.\n";
  std::cout << "[row] [col] For removeing.\n";
  std::cout << "Enter [u] For Undo.\n";
  std::cout << "Enter [r] For Redo.\n";

  std::string userInput = input::readString("Enter Your Action: ");
  item = evaluateInput(userInput);

  return item;
}

} // namespace ui
