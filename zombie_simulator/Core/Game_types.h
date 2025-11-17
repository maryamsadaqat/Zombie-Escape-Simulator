#pragma once

#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <random>
#include <string>
#include <limits>
#include "raylib.h"
#include "../Config/Config.h"

enum CellType {
    CELL_EMPTY = 0,
    CELL_WALL,
    CELL_EXIT,
    CELL_WEAPON,
    CELL_MEDKIT,
    CELL_HINT_SHORT,
    CELL_HINT_SAFE
};

struct Pos {
    int r, c;
    bool operator==(const Pos& other) const { return r == other.r && c == other.c; }
};

struct Game {
    std::vector<std::vector<CellType>> grid;
    Pos player;
    int playerHP = PLAYER_MAX_HP;

    std::stack<std::string> weapons;
    std::deque<std::string> medkits;

    std::vector<Pos> zombies;
    std::vector<Pos> exits;
    std::vector<Pos> lastPath;

    // UI/FX
    struct SplatFX { int r, c, ticks; };
    std::vector<SplatFX> splats;
    int healTicks = 0;

    bool safestMode = false;
    bool showPath = true;
    int  infectEveryNTurns = 3;
    int  turnNo = 0;
    bool skipEnemyThisTurn = false;
    int  zombieMoveEvery = 1;
    bool gameOver = false;
    bool gameWin = false;

    // Hint state
    bool hasHintShortest = false;
    bool hasHintSafest = false;
    enum LastHint { LH_NONE, LH_SHORT, LH_SAFE } lastHint = LH_NONE;

    // scoring state
    int  bestScore = std::numeric_limits<int>::max();
    int  lastScore = -1;
    bool scoreSaved = false;
    bool bestLoaded = false;

    std::mt19937 rng;

    Game();   // constructor defined in game_types.cpp
};
