#pragma once
#include "../Pathfinding/Pathfinding.h"
#include <string>

// Score helpers
int  LoadBestScoreFromFile(const char* path);
void SaveBestScoreToFile(const char* path, int best);
void FinalizeScore(Game& G);

// Turn + state
void PickupIfAny(Game& G);
void CheckWinLose(Game& G);
void ZombiesChase(Game& G);
void HandleZombieHits(Game& G);
void InfectionSpread(Game& G);
void RecomputePreview(Game& G);
void ResolveTurn(Game& G);
void NewGame(Game& G);

// Actions
bool UseWeapon(Game& G);
bool UseMedkit(Game& G);
void PlayerStep(Game& G, int dr, int dc);
