#pragma once
#include "../libs/utils/util.h"
#include "./grid.h"
#include "./log.h"
#include "./ui.h"
#include <array>
#include <fstream>
#include <iostream>
#include <map>

struct SubGridPositions {
  int s_row, s_column, e_row, e_column;
  SubGridPositions(int startingRow, int startingColumn, int endingRow,
                   int endingColumn) {
    s_row = startingRow;
    s_column = startingColumn;
    e_row = endingRow;
    e_column = endingColumn;
  }
};

class Generator {
private:
  static bool _isValidPlacement(sudokuGrid &grid, int row, int col, int n) {
    // Check row
    for (int j = 0; j < 9; j++) {
      if (grid[row][j] == n)
        return false;
    }

    // Check column
    for (int i = 0; i < 9; i++) {
      if (grid[i][col] == n)
        return false;
    }

    // Check 3x3 sub-grid
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (grid[startRow + i][startCol + j] == n)
          return false;
      }
    }
    return true;
  }

  // Recursive function to generate a complete Sudoku grid
  static bool generateFullGrid(sudokuGrid &grid, int row, int col) {
    // std::fstream logFile("log.txt", std::ios::out);

    // if (!logFile.is_open()) {
    //   std::cerr << "Error while opening log file.\n";
    //   logFile.close();
    //   exit(EXIT_FAILURE);
    // }

    // logFile << "[row]: " << row << ", [col]: " << col << '\n';

    if (row == 9) {
      // logFile.close();
      return true; // Base case: all rows filled
    }

    if (col == 9) {
      // logFile.close();
      return generateFullGrid(grid, row + 1, 0); // Move to next row
    }

    if (grid[row][col] != 0) {
      // logFile.close();
      return generateFullGrid(grid, row, col + 1); // Skip filled cells
    }

    for (int num = 1; num <= 9; num++) {
      bool validationResult = _isValidPlacement(grid, row, col, num);

      // { logFile << "Trying: " << num << ", Res: " << validationResult <<
      // '\n'; }

      if (validationResult) {
        grid[row][col] = num; // Place the number

        // {
        //   LOG::writeGrid(grid, logFile);
        //   logFile << "----------------------------------\n";
        // }

        if (generateFullGrid(grid, row, col + 1)) {
          // logFile.close();
          return true;
        }

        grid[row][col] = 0; // Backtrack

        // {
        //   logFile << "Backtracking...\n";
        //   logFile << "[row]: " << row << "[col]: " << col << '\n';
        //   LOG::writeGrid(grid, logFile);
        //   logFile << "----------------------------------\n";
        // }
      }
    }

    // logFile.close();
    return false;
  }

public:
  enum DifficultyLevel { easy = 20, medium = 25, hard = 30 };

  static sudokuGrid generateNewGrid(DifficultyLevel level) {
    sudokuGrid grid = {0};

    if (!generateFullGrid(grid, 0, 0)) {
      std::cerr << "Error while generating grid\n";
      exit(EXIT_FAILURE);
    }

    int totalCells = 81;
    int cellsToRemove = totalCells - (int)level;

    while (cellsToRemove > 0) {

      int row = util::randomNumber<int>(0, 8);
      int col = util::randomNumber<int>(0, 8);

      // check if the cell already removed
      if (grid[row][col] == 0)
        continue;

      grid[row][col] = 0;
      cellsToRemove--;
    }

    return grid;
  }
};