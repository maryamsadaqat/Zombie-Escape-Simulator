#include "../Config/Config.h"

// Config
const int GRID_ROWS = 20;
const int GRID_COLS = 20;

const int CELL_SIZE = 32;
const int HUD_HEIGHT = 160;
const int WIN_W = 1280;
const int WIN_H = GRID_ROWS * CELL_SIZE + HUD_HEIGHT;

const int ZOMBIE_START_COUNT = 3;
const int EXIT_COUNT_MIN = 3;
const int EXIT_COUNT_MAX = 4;

const int MAX_MEDKITS = 3;
const int MAX_WEAPONS = 3;

const int PLAYER_MAX_HP = 3;

// Colors
const Color COL_BG = Color{ 18, 20, 28, 255 };
const Color COL_EMPTY = Color{ 205, 205, 205, 255 };
const Color COL_WALL = Color{ 58, 62, 72, 255 };
const Color COL_PLAYER = Color{ 40, 200, 120, 255 };
const Color COL_ZOMBIE = Color{ 225, 64, 64, 255 };
const Color COL_EXIT = Color{ 70, 130, 230, 255 };
const Color COL_WEAPON = Color{ 230, 200, 40, 255 };
const Color COL_MEDKIT = Color{ 240, 240, 240, 255 };
const Color COL_GRIDLINE = Color{ 0, 0, 0, 80 };
const Color COL_PATH = Color{ 40, 220, 220, 160 };
const Color COL_HOVER = Color{ 255, 255, 255, 30 };
const Color COL_HINT = Color{ 160, 200, 255, 255 };

// Score file
const char* BEST_SCORE_FILE = "best_score.txt";
