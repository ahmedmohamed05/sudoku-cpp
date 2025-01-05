#pragma once
#include "../libs/utils/util.h"
#include <array>
#include <iostream>
#include <stack>

// (value, isPreFilled)
struct gridItem {
  int value;
  bool isPreFilled;
};

using sudokuGrid = std::array<std::array<gridItem, 9>, 9>;

class Grid {
private:
  bool _isValidPlacement(sudokuGrid &grid, int row, int col, int n) {
    // Check row
    for (int j = 0; j < 9; j++) {
      if (grid[row][j].value == n)
        return false;
    }

    // Check column
    for (int i = 0; i < 9; i++) {
      if (grid[i][col].value == n)
        return false;
    }

    // Check 3x3 sub-grid
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (grid[startRow + i][startCol + j].value == n)
          return false;
      }
    }
    return true;
  }

  // Recursive function to generate a complete Sudoku grid
  bool _generateFullGrid(sudokuGrid &grid, int row, int col) {

    if (row == 9)
      return true; // Base case: all rows filled

    if (col == 9)
      return _generateFullGrid(grid, row + 1, 0); // Move to next row

    if (grid[row][col].value != 0)
      return _generateFullGrid(grid, row, col + 1); // Skip filled cells

    for (int num = 1; num <= 9; num++) {
      bool validationResult = _isValidPlacement(grid, row, col, num);
      if (validationResult) {
        grid[row][col].value = num; // Place the number
        if (_generateFullGrid(grid, row, col + 1))
          return true;

        grid[row][col].value = 0; // Backtrack
      }
    }

    return false;
  }

  sudokuGrid _grid = {};
  bool _isSolved;

  struct _UserGridItem {
    int row, col, valued;
  };

  std::stack<_UserGridItem> _undos, _redos;

public:
  enum DifficultyLevel { easy = 1, medium, hard };

  Grid() {
    for (auto &row : _grid) {
      row.fill({0, false});
    }
  }

  Grid(DifficultyLevel level) { generateNewGrid(level); }

  const sudokuGrid &getGrid() const { return _grid; }

  // Returns false if the place is pre-filled or 0 > n or n > 9
  bool play(int row, int col, int n) {

    if (_grid[row][col].isPreFilled)
      return false;

    if (n < 0 || n > 9)
      return false;

    _grid[row][col].value = n;

    _undos.push({row, col, n});

    // TODO: check for sudoku rolls

    return true;
  }

  bool isSolved() const { return _isSolved; }

  void undo() {}

  void redo() {}

private:
  int _calculateCellsToRemove(DifficultyLevel level) {
    int min = 0, max = 1;
    switch (level) {
    case easy:
      min = 20;
      max = 29;
      break;
    case medium:
      min = 30;
      max = 39;
      break;
    case hard:
      min = 40;
      max = 49;
      break;
    }

    return util::randomNumber<int>(min, max);
  }

public:
  void generateNewGrid(DifficultyLevel level) {
    sudokuGrid grid = {{0, false}};

    if (!_generateFullGrid(grid, 0, 0)) {
      std::cerr << "Error while generating grid\n";
      exit(EXIT_FAILURE);
    }

    int cellsToRemove = _calculateCellsToRemove(level);

    if (cellsToRemove == -1) {
      std::cerr << "Something went Wrong, while removeing cells.\n";
      exit(EXIT_FAILURE);
    }

    while (cellsToRemove > 0) {

      int row = util::randomNumber<int>(0, 8);
      int col = util::randomNumber<int>(0, 8);

      // check if the cell already removed
      if (grid[row][col].value == 0)
        continue;

      grid[row][col].value = 0;
      grid[row][col].isPreFilled = false;
      cellsToRemove--;
    }

    _grid = grid;
  }
};