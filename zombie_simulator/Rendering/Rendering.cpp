#include "../Rendering/Rendering.h"
#include "../Config/Config.h"
#include "../Grid/Grid.h"
#include <string>

void DrawTextOutline(const char* txt, int x, int y, int size, Color mainCol, Color outline) {
    DrawText(txt, x - 1, y - 1, size, outline);
    DrawText(txt, x + 1, y - 1, size, outline);
    DrawText(txt, x - 1, y + 1, size, outline);
    DrawText(txt, x + 1, y + 1, size, outline);
    DrawText(txt, x, y, size, mainCol);
}

void DrawLegend(int x, int y) {
    const int rowH = 22;
    const int box = 14;
    const int pad = 10;
    const int w = 240;
    int h = pad * 2 + rowH * 8 + 18;

    Rectangle panel = { (float)x, (float)y, (float)w, (float)h };
    DrawRectangleRounded(panel, 0.20f, 6, Color{ 30, 30, 38, 220 });
    DrawRectangleLinesEx(panel, 1.5f, Color{ 0, 0, 0, 120 });

    DrawText("Legend", x + pad, y + 8, 20, RAYWHITE);

    int cy = y + 34;
    auto RowSimple = [&](Color col, const char* label) {
        DrawRectangle(x + pad, cy + 3, box, box, col);
        DrawText(label, x + pad + box + 8, cy, 18, Color{ 210, 210, 210, 255 });
        cy += rowH;
        };

    RowSimple(COL_PLAYER, "Player");
    RowSimple(COL_ZOMBIE, "Zombie");
    RowSimple(COL_EXIT, "Exit");
    RowSimple(COL_WEAPON, "Weapon");

    // Medkit row
    {
        int bx = x + pad, by = cy + 3;
        DrawRectangle(bx, by, box, box, COL_MEDKIT);
        DrawRectangle(bx + box / 2 - 1, by + 2, 3, box - 4, RED);
        DrawRectangle(bx + 2, by + box / 2 - 1, box - 4, 3, RED);
        DrawText("Medkit", x + pad + box + 8, cy, 18, Color{ 210, 210, 210, 255 });
        cy += rowH;
    }

    // Hint row
    {
        int bx = x + pad, by = cy + 3;
        DrawRectangle(bx, by, box, box, COL_HINT);
        const char* H = "H";
        int fs = (int)(box * 0.9f);
        if (fs < 10) fs = 10;
        if (fs > 18) fs = 18;
        int tw = MeasureText(H, fs);
        int tx = bx + (box - tw) / 2;
        int ty = by + (box - fs) / 2 - 1;
        DrawText(H, tx, ty, fs, BLACK);
        DrawText("Hint", x + pad + box + 8, cy, 18, Color{ 210, 210, 210, 255 });
        cy += rowH;
    }

    RowSimple(COL_EMPTY, "Walkable");
    RowSimple(COL_WALL, "Wall/Blocked");
}

void DrawEndgameOverlay(const Game& G) {
    if (!G.gameOver) return;

    const char* msg = G.gameWin ? "YOU ESCAPED!" : "YOU WERE CAUGHT!";
    const char* hint = "Press [R] to Restart or [Q] to Quit";

    Color panel = G.gameWin ? Color{ 40, 140, 80, 230 }
    : Color{ 180, 50, 50, 230 };

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    int fontTitle = 36, fontHint = 22;

    int tw = MeasureText(msg, fontTitle);
    int hw = MeasureText(hint, fontHint);

    int bw = std::max(tw, hw) + 100;
    int bh = 180;

    Rectangle rc = { (float)(sw - bw) / 2, (float)(sh - bh) / 2, (float)bw, (float)bh };
    DrawRectangleRounded(rc, 0.25f, 8, panel);
    DrawRectangleLinesEx(rc, 2.0f, Color{ 0, 0, 0, 120 });

    int tx = (int)(rc.x + (rc.width - tw) / 2);
    int ty = (int)(rc.y + 18);
    DrawText(msg, tx, ty, fontTitle, RAYWHITE);

    int score = (G.lastScore > 0) ? G.lastScore : G.turnNo;
    int best = (G.bestScore == std::numeric_limits<int>::max()) ? 0 : G.bestScore;

    std::string sc = "Your Score: " + std::to_string(score);
    std::string bs = "Best Score: " + std::to_string(best);

    int fsScore = 24;
    int scw = MeasureText(sc.c_str(), fsScore);
    int bsw = MeasureText(bs.c_str(), fsScore);

    int scx = (int)(rc.x + (rc.width - scw) / 2);
    int scy = ty + 46;

    int bsx = (int)(rc.x + (rc.width - bsw) / 2);
    int bsy = scy + 28;

    DrawText(sc.c_str(), scx, scy, fsScore, RAYWHITE);
    DrawText(bs.c_str(), bsx, bsy, fsScore, RAYWHITE);

    int hx = (int)(rc.x + (rc.width - hw) / 2);
    int hy = (int)(rc.y + rc.height - fontHint - 18);
    DrawText(hint, hx, hy, fontHint, Color{ 230, 230, 230, 255 });
}

void DrawGrid(const Game& G, int offsetX) {
    // background tiles + items
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {
            int x = offsetX + c * CELL_SIZE;
            int y = HUD_HEIGHT + r * CELL_SIZE;

            Color base = (G.grid[r][c] == CELL_WALL) ? COL_WALL : COL_EMPTY;
            DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, base);

            if (G.grid[r][c] == CELL_EXIT) {
                int pad = 4;
                int w = CELL_SIZE - pad * 2;
                DrawRectangle(x + pad, y + pad, w, w, COL_EXIT);

                float t = (sinf((float)GetTime() * 4.0f) * 0.5f) + 0.5f;
                float thick = 2.0f + 2.0f * t;
                unsigned char a = (unsigned char)(120 + 100 * t);
                Color glow = Color{ 90, 170, 255, a };
                Rectangle rc = { (float)(x + pad), (float)(y + pad), (float)w, (float)w };
                DrawRectangleLinesEx(rc, thick, glow);
            }
            else if (G.grid[r][c] == CELL_WEAPON) {
                DrawRectangle(x + 6, y + 6, CELL_SIZE - 12, CELL_SIZE - 12, COL_WEAPON);
            }
            else if (G.grid[r][c] == CELL_MEDKIT) {
                DrawRectangle(x + 6, y + 6, CELL_SIZE - 12, CELL_SIZE - 12, COL_MEDKIT);
                DrawRectangle(x + CELL_SIZE / 2 - 2, y + 8, 4, CELL_SIZE - 16, RED);
                DrawRectangle(x + 8, y + CELL_SIZE / 2 - 2, CELL_SIZE - 16, 4, RED);
            }
            else if (G.grid[r][c] == CELL_HINT_SHORT || G.grid[r][c] == CELL_HINT_SAFE) {
                DrawRectangle(x + 6, y + 6, CELL_SIZE - 12, CELL_SIZE - 12, COL_HINT);
                DrawText("H", x + CELL_SIZE / 2 - 6, y + CELL_SIZE / 2 - 10, 20, BLACK);
            }

            DrawRectangleLinesEx(
                Rectangle{ (float)x, (float)y, (float)CELL_SIZE, (float)CELL_SIZE },
                1, COL_GRIDLINE);
        }
    }

    // Path preview
    if (G.showPath) {
        for (auto& p : G.lastPath) {
            int x = offsetX + p.c * CELL_SIZE + 6;
            int y = HUD_HEIGHT + p.r * CELL_SIZE + 6;
            DrawRectangle(x, y, CELL_SIZE - 12, CELL_SIZE - 12, COL_PATH);
        }
    }

    // Zombies
    for (auto& z : G.zombies) {
        int x = offsetX + z.c * CELL_SIZE;
        int y = HUD_HEIGHT + z.r * CELL_SIZE;
        DrawRectangle(x + 2, y + 2, CELL_SIZE - 4, CELL_SIZE - 4, Color{ 0, 0, 0, 40 });
        DrawRectangle(x + 3, y + 3, CELL_SIZE - 6, CELL_SIZE - 6, COL_ZOMBIE);
    }

    // Weapon FX
    for (const auto& fx : G.splats) {
        int cx = offsetX + fx.c * CELL_SIZE + CELL_SIZE / 2;
        int cy = HUD_HEIGHT + fx.r * CELL_SIZE + CELL_SIZE / 2;
        float tt = (float)fx.ticks / 20.0f;
        float rr = (float)CELL_SIZE * (0.40f + 0.20f * (1.0f - tt));
        Color cc = Color{ 220, 60, 60, (unsigned char)(180 * tt) };
        DrawCircle(cx, cy, rr, cc);
    }

    // Player
    int px = offsetX + G.player.c * CELL_SIZE + CELL_SIZE / 2;
    int py = HUD_HEIGHT + G.player.r * CELL_SIZE + CELL_SIZE / 2;
    DrawCircle(px + 1, py + 1, CELL_SIZE * 0.38f, Color{ 0, 0, 0, 40 });
    DrawCircle(px, py, CELL_SIZE * 0.38f, COL_PLAYER);

    // Heal FX
    if (G.healTicks > 0) {
        float t = (float)G.healTicks / 25.0f;
        float rad = (float)CELL_SIZE * (0.45f + 0.30f * (1.0f - t));
        Color ring = Color{ 60, 220, 140, (unsigned char)(120 * t) };
        DrawCircleLines(px, py, rad, ring);
    }

    // Hover
    Vector2 m = GetMousePosition();
    int hc = (int)((m.x - offsetX) / CELL_SIZE);
    int hr = (int)((m.y - HUD_HEIGHT) / CELL_SIZE);
    if (InBounds(hr, hc)) {
        int hx = offsetX + hc * CELL_SIZE;
        int hy = HUD_HEIGHT + hr * CELL_SIZE;
        DrawRectangle(hx, hy, CELL_SIZE, CELL_SIZE, COL_HOVER);
    }

    // Legend
    int sh = GetScreenHeight();
    int legendH = 22 * 8 + 18 + 20;
    int legendY = HUD_HEIGHT + (sh - HUD_HEIGHT - legendH) / 2;
    DrawLegend(14, legendY);
}

void Chip(int x, int y, const char* text, Color bg, Color fg) {
    int padX = 8, padY = 4;
    int tw = MeasureText(text, 18);
    Rectangle rc = Rectangle{ (float)x, (float)y, (float)(tw + padX * 2), 28.0f };
    DrawRectangleRounded(rc, 0.3f, 6, bg);
    DrawText(text, x + padX, y + 5, 18, fg);
}

void DrawHUD(const Game& G, int /*gridOffsetX*/) {
    int sw = GetScreenWidth();

    const char* title = "Zombie Escape";
    int titleSize = 44;
    int tw = MeasureText(title, titleSize);
    int tx = (sw - tw) / 2;
    int ty = 10;

    DrawText(title, tx + 2, ty + 3, titleSize, Color{ 0, 0, 0, 180 });
    DrawText(title, tx - 2, ty - 1, titleSize, Color{ 0, 0, 0, 180 });
    DrawText(title, tx, ty, titleSize, Color{ 180, 220, 255, 255 });
    DrawText(title, tx, ty, titleSize, RAYWHITE);

    auto ChipSized = [&](int x, int y, const std::string& txt, Color bg, Color fg, int fs = 20) -> int {
        int padX = 10, padY = 6;
        int textW = MeasureText(txt.c_str(), fs);
        Rectangle rc = { (float)x, (float)y, (float)(textW + padX * 2), (float)(fs + padY * 2) };
        DrawRectangleRounded(rc, 0.30f, 6, bg);
        DrawText(txt.c_str(), x + padX, y + padY - 2, fs, fg);
        return (int)rc.width;
        };

    std::string hpTxt = "HP " + std::to_string(G.playerHP) + "/" + std::to_string(PLAYER_MAX_HP);
    std::string invTxt = "Weapons: " + std::to_string((int)G.weapons.size()) +
        "    Medkits: " + std::to_string((int)G.medkits.size());

    std::string hintTxt = "Hint: " +
        std::string(G.hasHintSafest && (G.lastHint == Game::LH_SAFE || !G.hasHintShortest)
            ? "Safest"
            : G.hasHintShortest ? "Shortest" : "None");

    auto measureChip = [&](const std::string& s, int fs = 20) { return MeasureText(s.c_str(), fs) + 20; };
    int wHP = measureChip(hpTxt);
    int wInv = measureChip(invTxt);
    int wHint = measureChip(hintTxt);

    int spacing = 55;
    int rowW = wHP + wInv + wHint + spacing * 2;
    int startX = (sw - rowW) / 2;
    int chipY = 56;

    Color hpBg = (G.playerHP > 1) ? Color{ 180, 240, 200, 255 }
    : Color{ 240, 180, 180, 255 };
    int x = startX;
    x += ChipSized(x, chipY, hpTxt, hpBg, BLACK, 20) + spacing;
    x += ChipSized(x, chipY, invTxt, Color{ 230, 230, 230, 255 }, BLACK, 20) + spacing;
    ChipSized(x, chipY, hintTxt, Color{ 200, 220, 255, 255 }, BLACK, 20);

    const char* l1 = "Arrows = Move      F = Use Weapon      H = Use Medkit      R = Restart      Q = Quit";
    const char* l2 = "Collect Hints to Reveal a Shortest or Safest Path";

    int fs = 20;
    int w1 = MeasureText(l1, fs);
    int w2 = MeasureText(l2, fs);

    int instrY1 = 100;
    int instrY2 = instrY1 + 26;

    DrawText(l1, (sw - w1) / 2, instrY1, fs, Color{ 190, 190, 190, 255 });
    DrawText(l2, (sw - w2) / 2, instrY2, fs, Color{ 190, 190, 190, 255 });

    std::string t = "Turns Taken: " + std::to_string(G.turnNo);
    int tw2 = MeasureText(t.c_str(), 22);
    DrawTextOutline(t.c_str(), sw - tw2 - 16, 16, 22, RAYWHITE);
}
