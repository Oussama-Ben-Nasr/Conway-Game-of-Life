#include <iostream>
#include <vector>
#include <unistd.h>>
#include <stdlib.h>

using namespace std;

bool valid_loc(int i, int j, int n) {
  return (i<n and j<n and i>=0 and j>=0);
};

int count_living_neighbors(vector<vector<bool>>& grid, int i, int j) {
  vector<pair<int, int>> directions({{1, 1}, {1, 0}, {1, -1}, {0, 1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}});
  int count = 0;

  for (pair<int, int>& direction: directions) {
    if (valid_loc(i + direction.first, j + direction.second, grid.size()) and grid[i + direction.first][j + direction.second])
      ++count;
  }

  return count;
};

vector<vector<bool>> step(vector<vector<bool>>& grid) {
  int n = grid.size();
  vector<vector<bool>> new_grid(n, vector<bool>(n));
  
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      int neigh_cnt = count_living_neighbors(grid, i, j);
      if (grid[i][j]) {
        if (neigh_cnt < 2) {
          // living cell < 2 living neigbors: dies
          new_grid[i][j] = false;
        } else if (neigh_cnt < 4) {
          // living cell 2 or 3 living neighbors: survives
          new_grid[i][j] = true;
        } else {
          // living cell > 3 living neighbors: dies
          new_grid[i][j] = false;
        }
      } else {
        // dead cell = 3 living neighbors: revieves
        if (neigh_cnt == 3) new_grid[i][j] = true;
      }
    }
  }
  return new_grid;
};

void render(vector<vector<bool>>& grid) {
  for (vector<bool>& row: grid) {
    for (bool cell: row) {
      cout << (cell ? "\033[1;31m*\033[0m": ".");
    }
    cout << endl;
  }
};

int main() {
  // finite grid first
  int n = 30;
  vector<vector<bool>> grid(n, vector<bool>(n));
  
  // add one floater
  grid[10][11] = true;
  grid[11][12] = true;
  grid[12][10] = true;
  grid[12][11] = true;
  grid[12][12] = true;

  // add one stick
  grid[10][5] = true;
  grid[11][5] = true;
  grid[12][5] = true;
  
  // main loop
  while(1) {
    render(grid);
    grid = step(grid);
    usleep(300000);
    system("clear");
  }
}