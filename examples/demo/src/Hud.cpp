#include "Hud.hpp"

#include "raylib.h"

namespace demo {

void drawHud(const char *effects, bool freeCam, bool menuHint)
{
    const int height = menuHint ? 160 : 132;

    DrawRectangle(8, 8, 520, height, Fade(BLACK, 0.55f));
    DrawRectangleLines(8, 8, 520, height, Fade(RAYWHITE, 0.15f));
    DrawText("Shimera - Démonstration", 18, 16, 18, RAYWHITE);
    // The list can get long once several effects are on: keep it inside the box.
    BeginScissorMode(18, 44, 500, 18);
        DrawText(TextFormat("Effets: %s", effects), 18, 44, 15, Color{120, 230, 255, 255});
    EndScissorMode();
    DrawText(TextFormat("Caméra: %s   [R] alterner", freeCam ? "Libre" : "Orbital"),
             18, 72, 14, Color{255, 210, 120, 255});
    if (freeCam) {
        DrawText("ZQSD bouger   Espace/Maj haut-bas", 18, 100, 14, GRAY);
    } else {
        DrawText("auto-orbital   molette = zoom", 18, 100, 14, GRAY);
    }
    if (menuHint)
        DrawText("[M] menu des shaders", 18, 128, 14, GRAY);
}

}
