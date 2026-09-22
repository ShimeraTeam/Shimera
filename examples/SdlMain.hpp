#pragma once

#include <SDL3/SDL.h>

/* A standalone SDL3 example */
class SdlMain {
public:
    SdlMain(const char* title, int width, int height);
    ~SdlMain();

    SdlMain(const SdlMain&) = delete;
    SdlMain& operator=(const SdlMain&) = delete;

    void run();

private:
    void draw() const;
    void drawBackground() const;
    void drawRectangle(const SDL_FRect& rect) const;
    void drawCircle(float centerX, float centerY, float radius, SDL_FColor color) const;

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture* m_background = nullptr;
    bool m_running = true;
};
