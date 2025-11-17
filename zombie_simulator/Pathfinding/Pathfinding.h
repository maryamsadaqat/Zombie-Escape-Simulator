#pragma once
#include "../Grid/Grid.h"

bool BFS_PlayerToExit(const Game& G, std::vector<Pos>& outPath);
int  DangerCost(const Game& G, int r, int c);
bool Dijkstra_SafestPath(const Game& G, std::vector<Pos>& outPath);
Pos  ZombieNextStepTowardPlayer(const Game& G, const Pos& z);
