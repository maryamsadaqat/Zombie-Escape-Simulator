#pragma once
#include "raylib.h"

// Grid & window config
extern const int GRID_ROWS;
extern const int GRID_COLS;

extern const int CELL_SIZE;
extern const int HUD_HEIGHT;
extern const int WIN_W;
extern const int WIN_H;

extern const int ZOMBIE_START_COUNT;
extern const int EXIT_COUNT_MIN;
extern const int EXIT_COUNT_MAX;

extern const int MAX_MEDKITS;
extern const int MAX_WEAPONS;

extern const int PLAYER_MAX_HP;

// Colors
extern const Color COL_BG;
extern const Color COL_EMPTY;
extern const Color COL_WALL;
extern const Color COL_PLAYER;
extern const Color COL_ZOMBIE;
extern const Color COL_EXIT;
extern const Color COL_WEAPON;
extern const Color COL_MEDKIT;
extern const Color COL_GRIDLINE;
extern const Color COL_PATH;
extern const Color COL_HOVER;
extern const Color COL_HINT;

// Score file
extern const char* BEST_SCORE_FILE;
