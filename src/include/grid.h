#pragma once
#include "../libs/utils/util.h"
#include "fstream"
#include <array>
#include <iostream>
#include <stack>
#include <unordered_set>

class Grid {

public:
  enum Actions { Add, Remove, Undo, Redo, Invalid };

  struct UserGridItem {
    int row, col, value;
    Actions action;
  };

  struct gridItem {
    int value;
    bool isPreFilled;
  };

  typedef std::array<std::array<gridItem, 9>, 9> sudokuGrid;

private:
  sudokuGrid _grid = {};
  bool _isSolved;

  bool _isValidPlacement(int row, int col, int n) {
    // Check row
    for (int j = 0; j < 9; j++) {
      if (_grid[row][j].value == n)
        return false;
    }

    // Check column
    for (int i = 0; i < 9; i++) {
      if (_grid[i][col].value == n)
        return false;
    }

    // Check 3x3 sub-grid
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (_grid[startRow + i][startCol + j].value == n)
          return false;
      }
    }
    return true;
  }

  // Recursive function to generate a complete Sudoku grid
  bool _generateFullGrid(int row, int col) {

    // We need to shuffle first row
    if (row == 0 && col == 9) {
      util::shuffle(_grid[0]);
      util::shuffle(_grid[0]);
    }

    if (row == 9)
      return true; // Base case: all rows filled

    if (col == 9)
      return _generateFullGrid(row + 1, 0); // Move to next row

    if (_grid[row][col].value != 0)
      return _generateFullGrid(row, col + 1); // Skip filled cells

    for (int num = 1; num <= 9; num++) {
      bool validationResult = _isValidPlacement(row, col, num);
      if (validationResult) {
        _grid[row][col] = {num, true}; // Place the item
        if (_generateFullGrid(row, col + 1))
          return true;

        _grid[row][col].value = 0; // Backtrack
      }
    }

    return false;
  }

  std::stack<UserGridItem> _undos, _redos;

public:
  enum DifficultyLevel { easy = 1, medium, hard };

  Grid() {
    for (auto &row : _grid) {
      row.fill({0, true});
    }
    _isSolved = false;
  }

  Grid(DifficultyLevel level) { generateNewGrid(level); }

  const sudokuGrid &getGrid() const { return _grid; }

private:
  bool _isValidRow(int row) {
    std::unordered_set<int> seen;
    for (const auto &item : _grid[row]) {
      if (seen.find(item.value) != seen.end())
        return false;

      seen.insert(item.value);
    }
    return true;
  }

  bool _isValidCol(int col) {
    std::unordered_set<int> seen;
    for (int i = 0; i < 9; i++) {
      const auto &item = _grid[i][col];
      if (seen.find(item.value) != seen.end())
        return false;

      seen.insert(item.value);
    }
    return true;
  }

  bool _isValidSubGrid(int row, int col) {
    std::unordered_set<int> seen;

    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;

    // Sub-grids are 3X3 2D-arrays
    for (int i = startRow; i < 3; i++) {
      for (int j = startCol; j < 3; j++) {

        const auto &item = _grid[i][j];

        if (seen.find(item.value) != seen.end())
          return false;

        seen.insert(item.value);
      }
    }

    return true;
  }

  bool _isValidSolution() {
    // check if the grid full first
    for (auto &row : _grid) {
      for (auto &col : row) {
        if (col.value == 0)
          return false;
      }
    }

    // Check rows and columns solutions
    for (int i = 0; i < 9; i++) {
      // if it's not valid row and col
      if (!(_isValidRow(i) && _isValidCol(i)))
        return false;

      // Check sub-grid
      for (int j = 0; j < 9; j++) {
        if (!_isValidSubGrid(i, j))
          return false;
      }
    }

    return true;
  }

public:
  // Returns false if the place is pre-filled or 0 > n or n > 9
  bool play(UserGridItem move, bool recordMove = true) {

    if (_isSolved)
      return false;

    auto [row, col, n, action] = move;

    if (action == Invalid)
      return false;

    if (action == Undo) {
      undo();
      return true;
    }

    if (action == Redo) {
      redo();
      return true;
    }

    if (!(input::isNumberBetween(row, 0, 8) &&
          input::isNumberBetween(col, 0, 8)))
      return false;

    if (_grid[row][col].isPreFilled)
      return false;

    if (action == Remove) {
      _grid[row][col].value = 0; // reset cell value
    } else {
      if (!input::isNumberBetween(n, 1, 9))
        return false;

      _grid[row][col].value = n; // put the value inside the cell
    }

    // record the move if it's not undo/redo and it's valid
    if (recordMove)
      _undos.push(move);

    if (_isValidSolution()) {

      while (_undos.size() > 0)
        _undos.pop();

      while (_redos.size() > 0)
        _redos.pop();

      _isSolved = true;
    }

    return true;
  }

  bool isSolved() const { return _isSolved; }

  void undo() {
    if (_undos.size() == 0)
      return;

    UserGridItem lastMove = _undos.top();

    _undos.pop();
    _redos.push(lastMove);

    // Reverse the action
    lastMove.action = lastMove.action == Add ? Remove : Add;

    // Play the move without recording it
    play(lastMove, false);
  }

  void redo() {
    if (_redos.size() == 0)
      return;

    UserGridItem move = _redos.top();
    _redos.pop();
    _undos.push(move);

    // Play the move without recording it
    play(move, false);
  }

private:
  int _calculateCellsToRemove(DifficultyLevel level) {
    int min = 0, max = 0;
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

    bool generatedSuccessfully = _generateFullGrid(0, 0);

    if (!generatedSuccessfully) {
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

      if (_grid[row][col].value != 0) {
        _grid[row][col].value = 0;
        _grid[row][col].isPreFilled = false;
        cellsToRemove--;
      }
    }
  }
};