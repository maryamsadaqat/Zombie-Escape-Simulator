#include "raylib.h"
#include "../Config/Config.h"
#include "../Game/Game.h"
#include "../Rendering/Rendering.h"

int main() {
    InitWindow(WIN_W, WIN_H, "Zombie Apocalypse: A Survival Simulation");
    SetTargetFPS(60);

    Game G;
    NewGame(G);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_Q)) break;

        bool acted = false;

        if (G.gameOver) {
            if (IsKeyPressed(KEY_R) || IsKeyPressed(KEY_Y)) {
                NewGame(G);
            }
            if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_N)) {
                break;
            }
        }
        else {
            if (IsKeyPressed(KEY_UP)) { PlayerStep(G, -1, 0); acted = true; }
            if (IsKeyPressed(KEY_DOWN)) { PlayerStep(G, 1, 0); acted = true; }
            if (IsKeyPressed(KEY_LEFT)) { PlayerStep(G, 0, -1); acted = true; }
            if (IsKeyPressed(KEY_RIGHT)) { PlayerStep(G, 0, 1); acted = true; }

            if (IsKeyPressed(KEY_F)) { if (UseWeapon(G)) acted = true; }
            if (IsKeyPressed(KEY_H)) { if (UseMedkit(G)) acted = true; }

            if (acted) ResolveTurn(G);
        }

        if (IsKeyPressed(KEY_R)) {
            NewGame(G);
        }

        int gridPixelW = GRID_COLS * CELL_SIZE;
        int gridOffsetX = (WIN_W - gridPixelW) / 2;
        if (gridOffsetX < 0) gridOffsetX = 0;

        // Update FX
        for (size_t i = 0; i < G.splats.size(); ) {
            if (--G.splats[i].ticks <= 0)
                G.splats.erase(G.splats.begin() + (ptrdiff_t)i);
            else
                ++i;
        }
        if (G.healTicks > 0) --G.healTicks;

        // Draw
        BeginDrawing();
        ClearBackground(COL_BG);

        DrawHUD(G, gridOffsetX);
        DrawGrid(G, gridOffsetX);
        DrawEndgameOverlay(G);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
