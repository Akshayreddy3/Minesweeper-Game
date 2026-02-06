#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <queue>

using namespace std;

class Minesweeper {
private:
    int rows, cols, totalMines;
    vector<vector<int>> grid;        // -1 = mine, 0-8 = number of adjacent mines
    vector<vector<bool>> revealed;   // track revealed cells
    vector<vector<bool>> flagged;    // track flagged cells
    bool gameOver;
    bool gameWon;
    
    // Directions for 8 adjacent cells (including diagonals)
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
public:
    Minesweeper(int r, int c, int mines) : rows(r), cols(c), totalMines(mines) {
        // Initialize 2D vectors
        grid.resize(rows, vector<int>(cols, 0));
        revealed.resize(rows, vector<bool>(cols, false));
        flagged.resize(rows, vector<bool>(cols, false));
        
        gameOver = false;
        gameWon = false;
        
        // Seed random number generator
        srand(time(nullptr));
        
        // Place mines randomly
        placeMines();
        
        // Calculate numbers for each cell
        calculateNumbers();
    }
    
    // DSA: Random placement algorithm with collision detection
    void placeMines() {
        int minesPlaced = 0;
        while (minesPlaced < totalMines) {
            int x = rand() % rows;
            int y = rand() % cols;
            
            // Check if cell doesn't already contain a mine
            if (grid[x][y] != -1) {
                grid[x][y] = -1;  // -1 represents a mine
                minesPlaced++;
            }
        }
    }
    
    // DSA: Graph traversal to calculate adjacent mine counts
    void calculateNumbers() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] != -1) {  // If not a mine
                    int count = 0;
                    
                    // Check all 8 adjacent cells
                    for (int k = 0; k < 8; k++) {
                        int newX = i + dx[k];
                        int newY = j + dy[k];
                        
                        // Check bounds and if adjacent cell is a mine
                        if (isValid(newX, newY) && grid[newX][newY] == -1) {
                            count++;
                        }
                    }
                    
                    grid[i][j] = count;
                }
            }
        }
    }
    
    // Helper function to check if coordinates are valid
    bool isValid(int x, int y) {
        return x >= 0 && x < rows && y >= 0 && y < cols;
    }
    
    // DSA: Flood Fill Algorithm using DFS
    void revealCell(int x, int y) {
        if (!isValid(x, y) || revealed[x][y] || flagged[x][y]) {
            return;
        }
        
        revealed[x][y] = true;
        
        // If clicked on a mine, game over
        if (grid[x][y] == -1) {
            gameOver = true;
            return;
        }
        
        // If cell has no adjacent mines, reveal all adjacent cells (flood fill)
        if (grid[x][y] == 0) {
            for (int k = 0; k < 8; k++) {
                int newX = x + dx[k];
                int newY = y + dy[k];
                revealCell(newX, newY);  // Recursive DFS
            }
        }
    }
    
    // Toggle flag on a cell
    void toggleFlag(int x, int y) {
        if (!isValid(x, y) || revealed[x][y]) {
            return;
        }
        
        flagged[x][y] = !flagged[x][y];
    }
    
    // Check if game is won
    bool checkWin() {
        int revealedCount = 0;
        int correctFlags = 0;
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (revealed[i][j] && grid[i][j] != -1) {
                    revealedCount++;
                }
                if (flagged[i][j] && grid[i][j] == -1) {
                    correctFlags++;
                }
            }
        }
        
        // Win condition: all non-mine cells revealed
        if (revealedCount == (rows * cols - totalMines)) {
            gameWon = true;
            return true;
        }
        
        return false;
    }
    
    // Display the game grid
    void displayGrid(bool showMines = false) {
        cout << "\n   ";
        for (int j = 0; j < cols; j++) {
            cout << setw(3) << j;
        }
        cout << "\n";
        
        for (int i = 0; i < rows; i++) {
            cout << setw(2) << i << " ";
            for (int j = 0; j < cols; j++) {
                if (flagged[i][j] && !showMines) {
                    cout << " F ";
                } else if (!revealed[i][j] && !showMines) {
                    cout << " . ";
                } else if (grid[i][j] == -1) {
                    cout << " * ";
                } else if (grid[i][j] == 0) {
                    cout << "   ";
                } else {
                    cout << " " << grid[i][j] << " ";
                }
            }
            cout << "\n";
        }
        cout << "\n";
    }
    
    // Display game statistics
    void displayStats() {
        int flagsUsed = 0;
        int cellsRevealed = 0;
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (flagged[i][j]) flagsUsed++;
                if (revealed[i][j]) cellsRevealed++;
            }
        }
        
        cout << "Mines: " << totalMines << " | Flags Used: " << flagsUsed 
             << " | Cells Revealed: " << cellsRevealed << "/" << (rows * cols - totalMines) << "\n";
    }
    
    // Main game loop
    void playGame() {
        cout << "Welcome to Minesweeper!\n";
        cout << "Commands:\n";
        cout << "  r x y - Reveal cell at (x,y)\n";
        cout << "  f x y - Flag/unflag cell at (x,y)\n";
        cout << "  q     - Quit game\n\n";
        
        while (!gameOver && !gameWon) {
            displayGrid();
            displayStats();
            
            cout << "Enter command: ";
            char command;
            cin >> command;
            
            if (command == 'q') {
                cout << "Thanks for playing!\n";
                return;
            }
            
            if (command == 'r' || command == 'f') {
                int x, y;
                cin >> x >> y;
                
                if (!isValid(x, y)) {
                    cout << "Invalid coordinates!\n";
                    continue;
                }
                
                if (command == 'r') {
                    revealCell(x, y);
                } else {
                    toggleFlag(x, y);
                }
                
                if (checkWin()) {
                    gameWon = true;
                }
            } else {
                cout << "Invalid command!\n";
            }
        }
        
        // Game ended - show final state
        displayGrid(true);
        
        if (gameWon) {
            cout << "🎉 Congratulations! You won! 🎉\n";
        } else {
            cout << "💥 Game Over! You hit a mine! 💥\n";
        }
    }
    
    // Get game state
    bool isGameOver() { return gameOver; }
    bool isGameWon() { return gameWon; }
};

// Function to get difficulty level
void getDifficultySettings(int& rows, int& cols, int& mines) {
    cout << "Select difficulty:\n";
    cout << "1. Beginner (9x9, 10 mines)\n";
    cout << "2. Intermediate (16x16, 40 mines)\n";
    cout << "3. Expert (30x16, 99 mines)\n";
    cout << "4. Custom\n";
    cout << "Enter choice (1-4): ";
    
    int choice;
    cin >> choice;
    
    switch (choice) {
        case 1:
            rows = 9; cols = 9; mines = 10;
            break;
        case 2:
            rows = 16; cols = 16; mines = 40;
            break;
        case 3:
            rows = 16; cols = 30; mines = 99;
            break;
        case 4:
            cout << "Enter rows: ";
            cin >> rows;
            cout << "Enter columns: ";
            cin >> cols;
            cout << "Enter number of mines: ";
            cin >> mines;
            
            // Validate custom settings
            if (rows <= 0 || cols <= 0 || mines <= 0 || mines >= rows * cols) {
                cout << "Invalid settings! Using beginner mode.\n";
                rows = 9; cols = 9; mines = 10;
            }
            break;
        default:
            cout << "Invalid choice! Using beginner mode.\n";
            rows = 9; cols = 9; mines = 10;
    }
}

int main() {
    int rows, cols, mines;
    
    cout << "=== MINESWEEPER GAME ===" << endl;
    cout << "C++ Implementation using DSA" << endl;
    cout << "=========================" << endl << endl;
    
    getDifficultySettings(rows, cols, mines);
    
    // Create and start the game
    Minesweeper game(rows, cols, mines);
    game.playGame();
    
    return 0;
}
