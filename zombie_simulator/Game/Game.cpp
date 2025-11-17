#include "../Game/Game.h"
#include "../Config/Config.h"
#include "../Grid/Grid.h"
#include "../Pathfinding/Pathfinding.h"
#include <fstream>
#include <algorithm>

int LoadBestScoreFromFile(const char* path) {
    std::ifstream fin(path);
    int v;
    if (fin && (fin >> v) && v > 0) return v;
    return std::numeric_limits<int>::max();
}

void SaveBestScoreToFile(const char* path, int best) {
    std::ofstream fout(path, std::ios::trunc);
    if (fout) fout << best;
}

void FinalizeScore(Game& G) {
    if (G.scoreSaved) return;
    G.lastScore = G.turnNo;

    if (G.gameWin && G.lastScore > 0 && G.lastScore < G.bestScore) {
        G.bestScore = G.lastScore;
        SaveBestScoreToFile(BEST_SCORE_FILE, G.bestScore);
    }
    G.scoreSaved = true;
}

void PickupIfAny(Game& G) {
    CellType& cell = G.grid[G.player.r][G.player.c];
    if (cell == CELL_WEAPON) {
        G.weapons.push("Grenade");
        cell = CELL_EMPTY;
    }
    else if (cell == CELL_MEDKIT) {
        G.medkits.push_back("Medkit");
        cell = CELL_EMPTY;
    }
    else if (cell == CELL_HINT_SHORT) {
        G.hasHintShortest = true;
        G.lastHint = Game::LH_SHORT;
        cell = CELL_EMPTY;
    }
    else if (cell == CELL_HINT_SAFE) {
        G.hasHintSafest = true;
        G.lastHint = Game::LH_SAFE;
        cell = CELL_EMPTY;
    }
}

void CheckWinLose(Game& G) {
    if (G.grid[G.player.r][G.player.c] == CELL_EXIT) {
        G.gameWin = true;
        G.gameOver = true;
    }
}

void ZombiesChase(Game& G) {
    std::vector<Pos> moved;
    moved.reserve(G.zombies.size());
    for (const auto& z : G.zombies)
        moved.push_back(ZombieNextStepTowardPlayer(G, z));
    G.zombies = moved;
}

void HandleZombieHits(Game& G) {
    int hitIndex = -1;
    for (size_t i = 0; i < G.zombies.size(); ++i) {
        if (G.zombies[i].r == G.player.r && G.zombies[i].c == G.player.c) {
            hitIndex = (int)i;
            break;
        }
    }
    if (hitIndex != -1) {
        if (G.playerHP > 0) G.playerHP--;
        G.zombies.erase(G.zombies.begin() + hitIndex);
        if (G.playerHP <= 0) {
            G.gameWin = false;
            G.gameOver = true;
        }
    }
}

void InfectionSpread(Game& G) {
    std::vector<Pos> add;
    add.reserve(256);
    for (const auto& z : G.zombies) {
        for (const auto& nb : Neigh4(z.r, z.c)) {
            if (!InBounds(nb.r, nb.c)) continue;
            if (G.grid[nb.r][nb.c] != CELL_EMPTY) continue;
            if (nb == G.player) continue;
            add.push_back(nb);
        }
    }

    std::sort(add.begin(), add.end(),
        [](const Pos& a, const Pos& b) {
            return (a.r == b.r) ? a.c < b.c : a.r < b.r;
        });
    add.erase(std::unique(add.begin(), add.end(),
        [](const Pos& a, const Pos& b) { return a.r == b.r && a.c == b.c; }),
        add.end());

    if (!add.empty()) {
        std::shuffle(add.begin(), add.end(), G.rng);
        int baseCap = 4;
        int scale = (int)G.zombies.size() / 6;
        int capNew = baseCap + std::min(scale, 4);
        int take = std::min((int)add.size(), capNew);
        for (int i = 0; i < take; ++i)
            if (!CellHasZombie(G, add[i].r, add[i].c))
                G.zombies.push_back(add[i]);
    }
}

void RecomputePreview(Game& G) {
    G.lastPath.clear();
    if (!G.showPath) return;
    if (!(G.hasHintShortest || G.hasHintSafest)) return;

    std::vector<Pos> tmp;
    bool ok = false;
    if (G.lastHint == Game::LH_SAFE && G.hasHintSafest)
        ok = Dijkstra_SafestPath(G, tmp);
    else if (G.lastHint == Game::LH_SHORT && G.hasHintShortest)
        ok = BFS_PlayerToExit(G, tmp);
    else if (G.hasHintSafest)
        ok = Dijkstra_SafestPath(G, tmp);
    else if (G.hasHintShortest)
        ok = BFS_PlayerToExit(G, tmp);

    if (ok) G.lastPath = tmp;
}

void ResolveTurn(Game& G) {
    if (G.gameOver) return;

    CheckWinLose(G);
    if (G.gameOver) { FinalizeScore(G); return; }

    bool skip = G.skipEnemyThisTurn;
    G.skipEnemyThisTurn = false;

    if (!skip) {
        if (G.zombieMoveEvery < 1) G.zombieMoveEvery = 1;
        if ((G.turnNo % G.zombieMoveEvery) == 0) ZombiesChase(G);

        HandleZombieHits(G);
        if (G.gameOver) { FinalizeScore(G); return; }

        if (G.infectEveryNTurns < 1) G.infectEveryNTurns = 1;
        if ((G.turnNo % G.infectEveryNTurns) == 0) InfectionSpread(G);
    }

    G.turnNo++;
    RecomputePreview(G);
}

void NewGame(Game& G) {
    G.gameOver = false;
    G.gameWin = false;
    G.turnNo = 0;
    G.playerHP = PLAYER_MAX_HP;

    while (!G.weapons.empty()) G.weapons.pop();
    G.medkits.clear();
    G.lastPath.clear();
    G.safestMode = false;
    G.showPath = true;
    G.infectEveryNTurns = 2;

    G.hasHintShortest = false;
    G.hasHintSafest = false;
    G.lastHint = Game::LH_NONE;

    G.scoreSaved = false;
    G.lastScore = -1;

    if (!G.bestLoaded) {
        G.bestScore = LoadBestScoreFromFile(BEST_SCORE_FILE);
        G.bestLoaded = true;
    }

    GenerateMaze(G);
    PlaceStuff(G);
    RecomputePreview(G);
}

bool UseWeapon(Game& G) {
    if (G.weapons.empty()) return false;

    int pr = G.player.r, pc = G.player.c;
    int R = 2;
    int killed = 0;

    std::vector<Pos> keep;
    keep.reserve(G.zombies.size());
    for (const auto& z : G.zombies) {
        bool inBlast = (std::abs(z.r - pr) <= R) && (std::abs(z.c - pc) <= R);
        if (inBlast) {
            G.splats.push_back({ z.r, z.c, 22 });
            killed++;
        }
        else {
            keep.push_back(z);
        }
    }
    if (killed == 0) return false;

    G.weapons.pop();
    G.zombies.swap(keep);
    G.skipEnemyThisTurn = true;
    return true;
}

bool UseMedkit(Game& G) {
    if (G.medkits.empty()) return false;
    if (G.playerHP >= PLAYER_MAX_HP) return false;
    G.medkits.pop_front();
    G.playerHP++;
    G.healTicks = 35;
    return true;
}

void PlayerStep(Game& G, int dr, int dc) {
    int nr = G.player.r + dr;
    int nc = G.player.c + dc;
    if (IsPassableForPlayer(G, nr, nc)) {
        G.player = Pos{ nr, nc };
        PickupIfAny(G);
    }
}
