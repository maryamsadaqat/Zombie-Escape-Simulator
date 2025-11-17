#include "../Grid/Grid.h"
#include <algorithm>
#include <random>

bool InBounds(int r, int c) {
    return r >= 0 && r < GRID_ROWS && c >= 0 && c < GRID_COLS;
}

std::vector<Pos> Neigh4(int r, int c) {
    static int dr[4] = { -1, 1, 0, 0 };
    static int dc[4] = { 0, 0,-1, 1 };
    std::vector<Pos> res;
    for (int k = 0; k < 4; k++) {
        int nr = r + dr[k], nc = c + dc[k];
        if (InBounds(nr, nc)) res.push_back(Pos{ nr, nc });
    }
    return res;
}

int Manh(const Pos& a, const Pos& b) {
    return abs(a.r - b.r) + abs(a.c - b.c);
}

bool CellHasZombie(const Game& G, int r, int c) {
    for (const auto& z : G.zombies)
        if (z.r == r && z.c == c) return true;
    return false;
}

bool IsPassableForPlayer(const Game& G, int r, int c) {
    if (!InBounds(r, c)) return false;
    if (G.grid[r][c] == CELL_WALL) return false;
    if (CellHasZombie(G, r, c)) return false;
    return true;
}

bool IsPassableForZombie(const Game& G, int r, int c) {
    if (!InBounds(r, c)) return false;
    CellType t = G.grid[r][c];
    if (t == CELL_WALL) return false;
    if (t == CELL_EXIT) return false;
    if (t == CELL_WEAPON || t == CELL_MEDKIT ||
        t == CELL_HINT_SHORT || t == CELL_HINT_SAFE) return false;
    return true;
}

std::vector<Pos> CollectEmptyCells(const Game& G) {
    std::vector<Pos> v;
    for (int r = 0; r < GRID_ROWS; r++)
        for (int c = 0; c < GRID_COLS; c++)
            if (G.grid[r][c] == CELL_EMPTY)
                v.push_back(Pos{ r, c });
    return v;
}

int RandRange(Game& G, int lo, int hi) {
    if (hi < lo) std::swap(lo, hi);
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(G.rng);
}

int RandIndex(Game& G, int sz) {
    if (sz <= 0) return 0;
    return RandRange(G, 0, sz - 1);
}

// Maze generation
void GenerateMaze(Game& G) {
    for (int r = 0; r < GRID_ROWS; r++)
        for (int c = 0; c < GRID_COLS; c++)
            G.grid[r][c] = CELL_WALL;

    std::uniform_int_distribution<int> distR(1, GRID_ROWS - 2);
    std::uniform_int_distribution<int> distC(1, GRID_COLS - 2);
    int sr = distR(G.rng);
    int sc = distC(G.rng);
    if (sr % 2 == 0) sr--;
    if (sc % 2 == 0) sc--;

    G.grid[sr][sc] = CELL_EMPTY;

    std::vector<Pos> frontier;
    auto pushFrontier = [&](int r, int c) {
        std::vector<Pos> cand = {
            Pos{r - 2, c}, Pos{r + 2, c},
            Pos{r, c - 2}, Pos{r, c + 2}
        };
        for (const auto& nb : cand) {
            if (InBounds(nb.r, nb.c) && G.grid[nb.r][nb.c] == CELL_WALL)
                frontier.push_back(nb);
        }
        };
    pushFrontier(sr, sc);

    while (!frontier.empty()) {
        int idx = RandIndex(G, (int)frontier.size());
        Pos cur = frontier[(size_t)idx];
        frontier.erase(frontier.begin() + idx);

        std::vector<Pos> carved;
        std::vector<Pos> cand = {
            Pos{cur.r - 2, cur.c}, Pos{cur.r + 2, cur.c},
            Pos{cur.r, cur.c - 2}, Pos{cur.r, cur.c + 2}
        };
        for (const auto& nb : cand) {
            if (InBounds(nb.r, nb.c) && G.grid[nb.r][nb.c] == CELL_EMPTY)
                carved.push_back(nb);
        }
        if (!carved.empty()) {
            int j = RandIndex(G, (int)carved.size());
            Pos to = carved[(size_t)j];

            int wr = (cur.r + to.r) / 2;
            int wc = (cur.c + to.c) / 2;

            G.grid[cur.r][cur.c] = CELL_EMPTY;
            G.grid[wr][wc] = CELL_EMPTY;

            pushFrontier(cur.r, cur.c);
        }
    }

    int extraOpen = (GRID_ROWS * GRID_COLS) / 20;
    std::uniform_int_distribution<int> rRand(0, GRID_ROWS - 1), cRand(0, GRID_COLS - 1);
    for (int k = 0; k < extraOpen; k++) {
        int rr = rRand(G.rng), cc = cRand(G.rng);
        if (InBounds(rr, cc)) G.grid[rr][cc] = CELL_EMPTY;
    }
}

Pos RandomFarFrom(const Game& G, const std::vector<Pos>& pool, const Pos& from, int minDist) {
    std::vector<Pos> candidates;
    candidates.reserve(pool.size());
    for (const auto& p : pool)
        if (Manh(p, from) >= minDist) candidates.push_back(p);

    if (candidates.empty()) {
        if (pool.empty()) return Pos{ 0, 0 };
        return pool[pool.size() / 2];
    }

    std::uniform_int_distribution<int> d(0, (int)candidates.size() - 1);
    return candidates[(size_t)d(const_cast<std::mt19937&>(G.rng))];
}

void PlaceStuff(Game& G) {
    auto empties = CollectEmptyCells(G);
    if (empties.size() < 50) {
        for (int r = 1; r < GRID_ROWS - 1; r++)
            for (int c = 1; c < GRID_COLS - 1; c++)
                if (G.grid[r][c] == CELL_WALL && ((r + c) % 3 == 0))
                    G.grid[r][c] = CELL_EMPTY;
        empties = CollectEmptyCells(G);
    }

    if (empties.empty()) {
        G.grid[1][1] = CELL_EMPTY;
        empties = CollectEmptyCells(G);
    }

    std::uniform_int_distribution<int> d(0, (int)empties.size() - 1);
    G.player = empties[(size_t)d(G.rng)];

    // Exits
    std::uniform_int_distribution<int> exitCountDist(EXIT_COUNT_MIN, EXIT_COUNT_MAX);
    int exitCount = exitCountDist(G.rng);
    G.exits.clear();
    for (int i = 0; i < exitCount; i++) {
        Pos e = RandomFarFrom(G, empties, G.player, (GRID_ROWS + GRID_COLS) / 2);
        if (!InBounds(e.r, e.c)) { i--; continue; }
        if (G.grid[e.r][e.c] == CELL_EXIT) { i--; continue; }
        G.grid[e.r][e.c] = CELL_EXIT;
        G.exits.push_back(e);
    }

    // Zombies
    G.zombies.clear();
    for (int i = 0; i < ZOMBIE_START_COUNT; i++) {
        Pos z = RandomFarFrom(G, empties, G.player, (GRID_ROWS + GRID_COLS) / 3);
        if (!InBounds(z.r, z.c)) { i--; continue; }
        if (G.grid[z.r][z.c] == CELL_EXIT) { i--; continue; }
        bool dup = false;
        for (const auto& zz : G.zombies)
            if (zz == z) dup = true;
        if (dup) { i--; continue; }
        G.zombies.push_back(z);
    }

    // Weapons
    int placedWeapons = 0;
    for (int tries = 0; tries < 600 && placedWeapons < MAX_WEAPONS; tries++) {
        Pos p = empties[(size_t)d(G.rng)];
        if (InBounds(p.r, p.c) &&
            G.grid[p.r][p.c] == CELL_EMPTY &&
            !(p == G.player) &&
            !CellHasZombie(G, p.r, p.c)) {
            G.grid[p.r][p.c] = CELL_WEAPON;
            placedWeapons++;
        }
    }

    // Medkits
    int placedMed = 0;
    for (int tries = 0; tries < 600 && placedMed < MAX_MEDKITS; tries++) {
        Pos p = empties[(size_t)d(G.rng)];
        if (InBounds(p.r, p.c) &&
            G.grid[p.r][p.c] == CELL_EMPTY &&
            !(p == G.player) &&
            !CellHasZombie(G, p.r, p.c)) {
            G.grid[p.r][p.c] = CELL_MEDKIT;
            placedMed++;
        }
    }

    // Hint tiles
    for (int tries = 0, placed = 0; tries < 800 && placed < 2; ++tries) {
        Pos p = empties[(size_t)d(G.rng)];
        if (!InBounds(p.r, p.c)) continue;
        if (p == G.player) continue;
        if (CellHasZombie(G, p.r, p.c)) continue;
        if (G.grid[p.r][p.c] != CELL_EMPTY) continue;
        if (Manh(p, G.player) < (GRID_ROWS + GRID_COLS) / 4) continue;

        if (placed == 0) {
            G.grid[p.r][p.c] = CELL_HINT_SHORT;
            placed++;
        }
        else {
            G.grid[p.r][p.c] = CELL_HINT_SAFE;
            placed++;
        }
    }
}
