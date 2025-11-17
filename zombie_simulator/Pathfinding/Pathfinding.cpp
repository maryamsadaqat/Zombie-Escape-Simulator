#include "../Pathfinding/Pathfinding.h"
#include <queue>
#include <tuple>
#include <limits>

bool BFS_PlayerToExit(const Game& G, std::vector<Pos>& outPath) {
    std::vector<std::vector<bool>> vis(GRID_ROWS, std::vector<bool>(GRID_COLS, false));
    std::vector<std::vector<Pos>> parent(GRID_ROWS, std::vector<Pos>(GRID_COLS, Pos{ -1, -1 }));
    std::queue<Pos> q;

    q.push(G.player);
    vis[G.player.r][G.player.c] = true;

    auto isExit = [&](int r, int c) { return G.grid[r][c] == CELL_EXIT; };
    Pos found{ -1, -1 };

    while (!q.empty()) {
        Pos cur = q.front(); q.pop();
        if (isExit(cur.r, cur.c)) { found = cur; break; }
        for (const auto& nb : Neigh4(cur.r, cur.c)) {
            if (!InBounds(nb.r, nb.c)) continue;
            if (vis[nb.r][nb.c]) continue;
            if (!IsPassableForPlayer(G, nb.r, nb.c)) continue;
            vis[nb.r][nb.c] = true;
            parent[nb.r][nb.c] = cur;
            q.push(nb);
        }
    }

    if (found.r == -1) return false;

    std::vector<Pos> path;
    Pos cur = found;
    while (!(cur == G.player)) {
        path.push_back(cur);
        cur = parent[cur.r][cur.c];
    }
    std::reverse(path.begin(), path.end());
    outPath = path;
    return true;
}

int DangerCost(const Game& G, int r, int c) {
    int cost = 0;
    for (const auto& z : G.zombies) {
        int d = Manh(Pos{ r, c }, z);
        if (d == 1) cost += 4;
        else if (d == 2) cost += 2;
    }
    return cost;
}

bool Dijkstra_SafestPath(const Game& G, std::vector<Pos>& outPath) {
    const int INF = 1000000000;
    std::vector<std::vector<int>> dist(GRID_ROWS, std::vector<int>(GRID_COLS, INF));
    std::vector<std::vector<Pos>> parent(GRID_ROWS, std::vector<Pos>(GRID_COLS, Pos{ -1, -1 }));

    auto cmp = [](std::tuple<int, int, int> a, std::tuple<int, int, int> b) {
        return std::get<0>(a) > std::get<0>(b);
        };
    std::priority_queue<
        std::tuple<int, int, int>,
        std::vector<std::tuple<int, int, int>>,
        decltype(cmp)
    > pq(cmp);

    dist[G.player.r][G.player.c] = 0;
    pq.push(std::tuple<int, int, int>(0, G.player.r, G.player.c));

    auto isExit = [&](int r, int c) { return G.grid[r][c] == CELL_EXIT; };
    Pos found{ -1, -1 };

    while (!pq.empty()) {
        auto top = pq.top(); pq.pop();
        int cd = std::get<0>(top);
        int rr = std::get<1>(top);
        int cc = std::get<2>(top);
        if (cd != dist[rr][cc]) continue;
        if (isExit(rr, cc)) { found = Pos{ rr, cc }; break; }

        for (const auto& nb : Neigh4(rr, cc)) {
            if (!IsPassableForPlayer(G, nb.r, nb.c)) continue;
            int step = 1 + DangerCost(G, nb.r, nb.c);
            int nd = cd + step;
            if (nd < dist[nb.r][nb.c]) {
                dist[nb.r][nb.c] = nd;
                parent[nb.r][nb.c] = Pos{ rr, cc };
                pq.push(std::tuple<int, int, int>(nd, nb.r, nb.c));
            }
        }
    }

    if (found.r == -1) return false;

    std::vector<Pos> path;
    Pos cur = found;
    while (!(cur == G.player)) {
        path.push_back(cur);
        cur = parent[cur.r][cur.c];
    }
    std::reverse(path.begin(), path.end());
    outPath = path;
    return true;
}

Pos ZombieNextStepTowardPlayer(const Game& G, const Pos& z) {
    if (z == G.player) return z;

    std::vector<std::vector<bool>> vis(GRID_ROWS, std::vector<bool>(GRID_COLS, false));
    std::vector<std::vector<Pos>> parent(GRID_ROWS, std::vector<Pos>(GRID_COLS, Pos{ -1, -1 }));
    std::queue<Pos> q;
    q.push(z);
    vis[z.r][z.c] = true;
    bool found = false;

    while (!q.empty()) {
        Pos cur = q.front(); q.pop();
        if (cur == G.player) { found = true; break; }
        for (const auto& nb : Neigh4(cur.r, cur.c)) {
            if (!IsPassableForZombie(G, nb.r, nb.c)) continue;
            if (vis[nb.r][nb.c]) continue;
            vis[nb.r][nb.c] = true;
            parent[nb.r][nb.c] = cur;
            q.push(nb);
        }
    }

    if (!found) return z;

    Pos cur = G.player;
    Pos prev = Pos{ -1, -1 };
    while (!(cur == z)) {
        prev = cur;
        cur = parent[cur.r][cur.c];
    }
    return (prev.r == -1) ? z : prev;
}
