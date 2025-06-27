#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <list>

using namespace std;

#define UNASSIGNED 0

class SudokuGraph {
public:
    SudokuGraph(int gridSize);
    void printGrid();
    bool isSafe(int row, int col, int num);
    bool solveSudoku();
    void giveClue(int row, int col);
    void modifyGrid();
    void inputGrid();
    void buildGraph();

private:
    vector<vector<int>> grid;
    vector<list<int>> adjList;
    int gridSize;
    int subGridRows;
    int subGridCols;
    bool findUnassignedLocation(int& row, int& col);
};

SudokuGraph::SudokuGraph(int gridSize) : gridSize(gridSize) {
    grid = vector<vector<int>>(gridSize, vector<int>(gridSize, UNASSIGNED));
    subGridRows = sqrt(gridSize);
    subGridCols = (subGridRows * subGridRows == gridSize) ? subGridRows : gridSize / subGridRows;
    adjList.resize(gridSize * gridSize);
    buildGraph();
}

void SudokuGraph::printGrid() {
    for (int row = 0; row < gridSize; row++) {
        if (row % subGridRows == 0 && row != 0)
            cout << string(gridSize * 2 + subGridCols - 1, '-') << endl;
        for (int col = 0; col < gridSize; col++) {
            if (col % subGridCols == 0 && col != 0)
                cout << " | ";
            if (grid[row][col] == UNASSIGNED)
                cout << ". ";
            else
                cout << grid[row][col] << " ";
        }
        cout << endl;
    }
}

bool SudokuGraph::isSafe(int row, int col, int num) {
    for (int x = 0; x < gridSize; x++) {
        if (grid[row][x] == num || grid[x][col] == num)
            return false;
    }

    int startRow = row - row % subGridRows, startCol = col - col % subGridCols;
    for (int i = 0; i < subGridRows; i++)
        for (int j = 0; j < subGridCols; j++)
            if (grid[i + startRow][j + startCol] == num)
                return false;

    return true;
}

bool SudokuGraph::findUnassignedLocation(int& row, int& col) {
    for (row = 0; row < gridSize; row++) {
        for (col = 0; col < gridSize; col++) {
            if (grid[row][col] == UNASSIGNED)
                return true;
        }
    }
    return false;
}

bool SudokuGraph::solveSudoku() {
    int row, col;
    if (!findUnassignedLocation(row, col))
        return true;

    for (int num = 1; num <= gridSize; num++) {
        if (isSafe(row, col, num)) {
            grid[row][col] = num;
            if (solveSudoku())
                return true;
            grid[row][col] = UNASSIGNED;
        }
    }
    return false;
}

void SudokuGraph::giveClue(int row, int col) {
    vector<int> notSafeNums;
    for (int num = 1; num <= gridSize; num++) {
        if (!isSafe(row, col, num)) {
            notSafeNums.push_back(num);
        }
    }

    cout << "Clue for cell (" << row + 1 << "," << col + 1 << "):" << endl;
    srand(time(0));
    int cluesToShow = notSafeNums.size() / 2;
    for (int i = 0; i < cluesToShow; i++) {
        int index = rand() % notSafeNums.size();
        cout << "Number " << notSafeNums[index] << " cannot be placed here." << endl;
        notSafeNums.erase(notSafeNums.begin() + index);
    }
}

void SudokuGraph::modifyGrid() {
    char choice;
    int row, col, num;

    while (true) {
        cout << "Current Sudoku grid:" << endl;
        printGrid();

        cout << "Choose an action (i: insert, d: delete, s: solve, q: quit, c: clue): ";
        cin >> choice;

        if (choice == 'i') {
            cout << "Enter row (1-" << gridSize << "): ";
            cin >> row;
            cout << "Enter column (1-" << gridSize << "): ";
            cin >> col;
            cout << "Enter number (1-" << gridSize << "): ";
            cin >> num;

            if (row >= 1 && row <= gridSize && col >= 1 && col <= gridSize && num >= 1 && num <= gridSize) {
                grid[row - 1][col - 1] = num;
                cout << "Number inserted successfully! Remember to solve Sudoku to verify." << endl;
            } else {
                cout << "Invalid input. Number must be between 1 and " << gridSize << "." << endl;
            }
        } else if (choice == 'd') {
            cout << "Enter row (1-" << gridSize << ") of the number to delete: ";
            cin >> row;
            cout << "Enter column (1-" << gridSize << ") of the number to delete: ";
            cin >> col;

            if (row >= 1 && row <= gridSize && col >= 1 && col <= gridSize) {
                grid[row - 1][col - 1] = UNASSIGNED;
                cout << "Number deleted successfully!" << endl;
            } else {
                cout << "Invalid input. Try again." << endl;
            }
        } else if (choice == 's') {
            if (solveSudoku()) {
                cout << "Sudoku solved successfully!" << endl;
                printGrid();
            } else {
                cout << "No solution exists for the given Sudoku puzzle." << endl;
            }
            break;
        } else if (choice == 'q') {
            break;
        } else if (choice == 'c') {
            cout << "Enter row (1-" << gridSize << "): ";
            cin >> row;
            cout << "Enter column (1-" << gridSize << "): ";
            cin >> col;

            if (row >= 1 && row <= gridSize && col >= 1 && col <= gridSize) {
                giveClue(row - 1, col - 1);
            } else {
                cout << "Invalid input. Try again." << endl;
            }
        } else {
            cout << "Invalid choice. Try again." << endl;
        }
    }
}

void SudokuGraph::inputGrid() {
    cout << "Enter the initial Sudoku grid (use 0 for empty cells):" << endl;
    for (int row = 0; row < gridSize; row++) {
        for (int col = 0; col < gridSize; col++) {
            while (true) {
                cout << "Enter value for cell (" << row + 1 << "," << col + 1 << "): ";
                cin >> grid[row][col];

                if (cin.fail() || grid[row][col] < 0 || grid[row][col] > gridSize) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number between 0 and " << gridSize << "." << endl;
                } else {
                    break;
                }
            }
        }
    }
}

void SudokuGraph::buildGraph() {
    for (int row = 0; row < gridSize; row++) {
        for (int col = 0; col < gridSize; col++) {
            int node = row * gridSize + col;
            // Connect rows
            for (int k = 0; k < gridSize; k++) {
                if (k != col) {
                    adjList[node].push_back(row * gridSize + k);
                }
            }
            // Connect columns
            for (int k = 0; k < gridSize; k++) {
                if (k != row) {
                    adjList[node].push_back(k * gridSize + col);
                }
            }
            // Connect subgrid
            int startRow = row - row % subGridRows;
            int startCol = col - col % subGridCols;
            for (int i = 0; i < subGridRows; i++) {
                for (int j = 0; j < subGridCols; j++) {
                    int neighbor = (startRow + i) * gridSize + (startCol + j);
                    if (neighbor != node) {
                        adjList[node].push_back(neighbor);
                    }
                }
            }
        }
    }
}

int main() {
    int gridSize;
    cout << "Enter the size of the Sudoku grid (e.g., 9 for 9x9, 16 for 16x16): ";
    cin >> gridSize;

    SudokuGraph sudoku(gridSize);
    sudoku.inputGrid();
    sudoku.modifyGrid();

    return 0;
}