#pragma once
#include "../Core/Game_types.h"

void DrawTextOutline(const char* txt, int x, int y, int size, Color mainCol, Color outline = BLACK);
void DrawLegend(int x, int y);
void DrawEndgameOverlay(const Game& G);
void DrawGrid(const Game& G, int offsetX);
void Chip(int x, int y, const char* text, Color bg, Color fg = BLACK);
void DrawHUD(const Game& G, int gridOffsetX);
