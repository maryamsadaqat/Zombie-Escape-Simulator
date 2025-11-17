#pragma once
#include "../Core/Game_types.h"

// Basic helpers
bool InBounds(int r, int c);
std::vector<Pos> Neigh4(int r, int c);
int Manh(const Pos& a, const Pos& b);

bool CellHasZombie(const Game& G, int r, int c);
bool IsPassableForPlayer(const Game& G, int r, int c);
bool IsPassableForZombie(const Game& G, int r, int c);

std::vector<Pos> CollectEmptyCells(const Game& G);

int RandRange(Game& G, int lo, int hi);
int RandIndex(Game& G, int sz);

// Placement helpers
Pos RandomFarFrom(const Game& G, const std::vector<Pos>& pool, const Pos& from, int minDist);

// Maze + content
void GenerateMaze(Game& G);
void PlaceStuff(Game& G);
