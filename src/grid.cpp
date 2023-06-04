#include <stdlib.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono;

bool within_grid(int i, int j, int m, int n) {
  return (i < m and j < n and i >= 0 and j >= 0);
};

int count_living_neighbors(vector<vector<bool>>& grid, int i, int j) {
  vector<pair<int, int>> directions(
      {{1, 1}, {1, 0}, {1, -1}, {0, 1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}});
  int count = 0;

  for (pair<int, int>& direction : directions) {
    // if (valid_loc(i + direction.first, j + direction.second, grid.size(),
    // grid[0].size()) and grid[(i + direction.first)][(j + direction.second)])
    if (grid[(i + direction.first + 2 * grid.size()) % grid.size()]
            [(j + direction.second + 2 * grid[0].size()) % grid[0].size()])
      ++count;
  }

  return count;
};

vector<vector<bool>> step(vector<vector<bool>>& grid) {
  int m = grid.size();
  int n = grid[0].size();
  vector<vector<bool>> new_grid(m, vector<bool>(n));

  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      int neigh_cnt = count_living_neighbors(grid, i, j);
      if (grid[i][j] and (neigh_cnt == 2 or neigh_cnt == 3)) {
        // living cell 2 or 3 living neighbors: survives
        new_grid[i][j] = true;
      } else {
        // dead cell = 3 living neighbors: revieves
        if (neigh_cnt == 3) new_grid[i][j] = true;
      }
    }
  }
  return new_grid;
};

void render(vector<vector<bool>>& grid) {
  for (vector<bool>& row : grid) {
    for (bool cell : row) {
      cout << (cell ? "\033[1;31m#\033[0m" : "#");
    }
    cout << endl;
  }
};

int main() {
  // finite grid first
  int n = 40, m = 25;
  vector<vector<bool>> grid(m, vector<bool>(n));

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
  while (1) {
    render(grid);
    auto start = high_resolution_clock::now();
    grid = step(grid);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << duration.count() << endl;
    usleep(1000);
    system("clear");
  }
}
