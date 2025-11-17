#include "../Core/Game_types.h"
#include <ctime>

Game::Game() {
    rng.seed((unsigned)time(nullptr));
    grid.assign(GRID_ROWS, std::vector<CellType>(GRID_COLS, CELL_WALL));
}
