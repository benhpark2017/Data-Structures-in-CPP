/**
 * Solutions to Chapter 6, Section 7, Exercise 7 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++. 
 * 
 * Task 7:
 * Let G[n][n] be a wiring grid. G[i][j] > 0 represents a grid position that is
 * blocked; G[i][j] = 0 represents an unblocked position. Assume that positions
 * [a][b] and [c][d] are blocked positions. A path from [a][b] to [b][c] is a 
 * sequence of grid positions such that 
 * 
 * 1. [a][b] and [c][d] are, respectively, the first and last positions on the
 *    path;
 * 2. successive positions of the sequence are vertically or horizontally 
 *    adjacent in the grid;
 * 3. all positions of the sequence other than the first and last are unblocked
 *    positions.
 * 
 * The length of the path is the number of grid positions on the path. We want
 * to connect positions [a][b] and [c][d] by a wire of shortest length. The wire
 * path is a shortest grid path between these two vertices. Lee's algorithm for
 * this works in the following steps:
 * 
 * 1. [Forward step] Start a breadth-first search from position [a][b], labeling
 *    unblocked positions by their shortest distance from [a][b]. To avoid
 *    conflicts with existing labels, use negative labels. The labeling stops 
 *    when [c][d] is reached.
 * 2. [Backtrace] Use the labels of the forward step to label the shortest path 
 *    between [a][b] and [c][d], using the unique label w>0 for the wire. For
 *    this, start at position [c][d].
 * 3. [Clean-up] Change the remaining labels to 0.
 * 
 * Write C++ functions for each of the three steps of Lee's algorithm.
 */

#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#include <iomanip>
#include <limits>

// Structure to represent a grid position
struct Position {
    int row, col;
    Position(int r, int c) : row(r), col(c) {}
};

// Forward step: BFS to label distances from start position
void forwardStep(std::vector<std::vector<int>>& G, Position start, Position end) {
    int n = G.size();
    std::queue<Position> Q;
    Q.push(start);
    
    // Save original value of start position
    int startValue = G[start.row][start.col];
    
    // Label start position with -1
    G[start.row][start.col] = -1;
    
    // Direction arrays for 4-way movement
    int dr[] = {-1, 1, 0, 0};  // up, down, left, right
    int dc[] = {0, 0, -1, 1};
    
    while (!Q.empty()) {
        Position current = Q.front();
        Q.pop();
        
        // Stop if we've reached the end position
        if (current.row == end.row && current.col == end.col) {
            // Restore start position's value
            G[start.row][start.col] = startValue;
            return;
        }
        
        // Check all four directions
        for (int i = 0; i < 4; i++) {
            int newRow = current.row + dr[i];
            int newCol = current.col + dc[i];
            
            // Check if new position is within grid and unvisited (== 0)
            if (newRow >= 0 && newRow < n && newCol >= 0 && newCol < n && 
                G[newRow][newCol] == 0) {
                // Label with distance (negative to avoid conflicts)
                G[newRow][newCol] = G[current.row][current.col] - 1;
                Q.push(Position(newRow, newCol));
            }
        }
    }
    
    // Restore start position's value
    G[start.row][start.col] = startValue;
}

// Backtrace step: Label the wire path with positive value w
void backtraceStep(std::vector<std::vector<int>>& G, Position start, Position end, int w) {
    Position current = end;
    
    // Direction arrays for 4-way movement
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    
    // Save original values of start and end positions
    int startValue = G[start.row][start.col];
    int endValue = G[end.row][end.col];
    
    while (!(current.row == start.row && current.col == start.col)) {
        // Label current position as wire
        G[current.row][current.col] = w;
        
        // Find neighbor with largest negative label (closest to zero)
        Position next = current;
        int maxLabel = -std::numeric_limits<int>::max();  // Initialize to smallest possible value
        bool foundNext = false;
        
        for (int i = 0; i < 4; i++) {
            int newRow = current.row + dr[i];
            int newCol = current.col + dc[i];
            
            if (newRow >= 0 && newRow < G.size() && newCol >= 0 && newCol < G.size()) {
                if (G[newRow][newCol] < 0 && G[newRow][newCol] > maxLabel) {
                    maxLabel = G[newRow][newCol];
                    next = Position(newRow, newCol);
                    foundNext = true;
                }
            }
        }
        
        if (!foundNext) {
            // If no valid next position is found, something went wrong
            break;
        }
        
        current = next;
    }
    
    // Restore start and end positions' values
    G[start.row][start.col] = startValue;
    G[end.row][end.col] = endValue;
}

// Clean-up step: Reset all negative labels to 0
void cleanupStep(std::vector<std::vector<int>>& G) {
    for (int i = 0; i < G.size(); i++) {
        for (int j = 0; j < G.size(); j++) {
            if (G[i][j] < 0) {
                G[i][j] = 0;
            }
        }
    }
}

// Main function to run Lee's algorithm
void leesAlgorithm(std::vector<std::vector<int>>& G, Position start, Position end, int wireLabel) {
    forwardStep(G, start, end);
    backtraceStep(G, start, end, wireLabel);
    cleanupStep(G);
}

// Utility function to print the grid
void printGrid(const std::vector<std::vector<int>>& G) {
    std::cout << std::string(4 * G.size() + 1, '-') << std::endl;
    for (const auto& row : G) {
        std::cout << "|";
        for (int val : row) {
            if (val == 0) {
                std::cout << "   |"; // Empty space
            } else {
                std::cout << std::setw(3) << val << "|"; // Right-aligned number
            }
        }
        std::cout << std::endl << std::string(4 * G.size() + 1, '-') << std::endl;
    }
    std::cout << std::endl;
}


int main() {
    // Create a larger grid with multiple obstacles and potential paths
    // 1 represents obstacles (blocked positions)
    // 0 represents empty spaces
    // We'll try to connect two points avoiding the obstacles
    std::vector<std::vector<int>> grid = {
        {1, 0, 0, 0, 0, 1, 0, 1},
        {0, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 0},
        {0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 1, 0, 1},
        {1, 1, 0, 0, 0, 0, 1, 0},
        {0, 0, 1, 0, 1, 0, 0, 0},
        {1, 0, 0, 0, 0, 1, 0, 1}
    };

    // Define multiple test cases
    std::vector<std::pair<Position, Position>> testCases = {
        {Position(0, 1), Position(7, 6)},  // Diagonal corners
        {Position(3, 0), Position(3, 7)},  // Horizontal path with obstacles
        {Position(0, 3), Position(7, 3)}   // Vertical path with obstacles
    };

    std::cout << "Initial Grid Configuration:" << std::endl;
    printGrid(grid);

    // Run Lee's algorithm for each test case
    for (int i = 0; i < testCases.size(); i++) {
        std::cout << "\nTest Case " << i + 1 << ":" << std::endl;
        std::cout << "Finding path from [" << testCases[i].first.row << "][" 
                  << testCases[i].first.col << "] to [" 
                  << testCases[i].second.row << "][" 
                  << testCases[i].second.col << "]" << std::endl;

        // Create a copy of the original grid for this test case
        std::vector<std::vector<int>> gridCopy = grid;

        // Run Lee's algorithm
        // Using i + 2 as wire label to distinguish different paths
        leesAlgorithm(gridCopy, testCases[i].first, testCases[i].second, i + 2);

        std::cout << "\nResulting Grid (Path labeled with " << i + 2 << "):" << std::endl;
        printGrid(gridCopy);

        // Count the length of the wire (number of positions labeled with i + 2)
        int wireLength = 0;
        for (const auto& row : gridCopy) {
            for (int val : row) {
                if (val == i + 2) wireLength++;
            }
        }
        std::cout << "Wire length: " << wireLength << " units" << std::endl;
    }

    std::cout << std::endl << "End of program." << std::endl;

    return 0;
}
