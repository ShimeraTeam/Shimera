#include "SdlMain.hpp"

#include "effects/Distortion.hpp"
#include "effects/HDRBloom.hpp"
#include "effects/Vignette.hpp"

#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include <cmath>
#include <iostream>
#include <numbers>
#include <vector>

namespace {

constexpr int kWindowWidth = 960;
constexpr int kWindowHeight = 540;

constexpr const char* kBackgroundPath = "res/imgs/pattern_checkerboard_gray.png";

constexpr float kBackgroundScale = 0.75f;

constexpr int kCircleSegments = 96;

} // namespace

SdlMain::~SdlMain() {
    m_fx.reset();
    m_scene.reset();
    m_context.reset();

    if (m_background != nullptr)
        SDL_DestroyTexture(m_background);
    if (m_renderer != nullptr)
        SDL_DestroyRenderer(m_renderer);
    if (m_window != nullptr)
        SDL_DestroyWindow(m_window);

    SDL_Quit();
}

bool SdlMain::init(const char* title, const int width, const int height) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return false;
    }

    m_window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
    if (m_window == nullptr) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
        return false;
    }

    // "opengl" is needed for Shimera
    m_renderer = SDL_CreateRenderer(m_window, "opengl");
    if (m_renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer(opengl) failed: " << SDL_GetError() << '\n';
        return false;
    }

    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderVSync(m_renderer, 1);

    if (!loadBackground(kBackgroundPath))
        return false;

    m_context.emplace(shimera::Context::create());
    std::cout << shimera::Context::getRendererInfo() << '\n';

    m_scene.emplace(*m_renderer, width, height);

    m_fx.emplace(width, height);
    m_fx->add<shimera::Vignette>(1.0f, 0.45f, 0.35f)
        .add<shimera::Distortion>()
        .add<shimera::HDRBloom>();

    return true;
}

bool SdlMain::loadBackground(const char* path) {
    SDL_Surface* image = IMG_Load(path);
    if (image == nullptr) {
        std::cerr << "IMG_Load(\"" << path << "\") failed: " << SDL_GetError() << '\n';
        return false;
    }

    m_background = SDL_CreateTextureFromSurface(m_renderer, image);
    SDL_DestroySurface(image);
    if (m_background == nullptr) {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << '\n';
        return false;
    }

    SDL_SetTextureScaleMode(m_background, SDL_SCALEMODE_NEAREST);
    return true;
}

void SdlMain::run() {
    while (m_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT
                || (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE))
                m_running = false;
            else if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
                resize(event.window.data1, event.window.data2);
        }

        m_fx->get<shimera::Distortion>().withTime(static_cast<float>(SDL_GetTicks()) / 300);

        draw();
    }
}

void SdlMain::resize(const int width, const int height) {
    if (width <= 0 || height <= 0)
        return;

    m_scene->resize(width, height);
    m_fx->resize(width, height);
}

void SdlMain::draw() {
    m_scene->begin();

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);

    drawBackground();
    drawRectangle(SDL_FRect{140.0f, 170.0f, 280.0f, 210.0f});
    drawCircle(670.0f, 275.0f, 115.0f, SDL_FColor{0.16f, 0.71f, 0.95f, 1.0f});

    m_scene->end();

    m_fx->render(*m_scene);

    SDL_RenderPresent(m_renderer);
}

void SdlMain::drawBackground() const {
    int width = 0;
    int height = 0;
    SDL_GetRenderOutputSize(m_renderer, &width, &height);

    const SDL_FRect destination{0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)};
    SDL_RenderTextureTiled(m_renderer, m_background, nullptr, kBackgroundScale, &destination);
}

void SdlMain::drawRectangle(const SDL_FRect& rect) const {
    SDL_SetRenderDrawColor(m_renderer, 214, 79, 56, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(m_renderer, &rect);

    SDL_SetRenderDrawColor(m_renderer, 255, 236, 214, SDL_ALPHA_OPAQUE);
    SDL_RenderRect(m_renderer, &rect);
}

void SdlMain::drawCircle(const float centerX, const float centerY, const float radius,
                         const SDL_FColor color) const {
    std::vector<SDL_Vertex> vertices;
    vertices.reserve(static_cast<size_t>(kCircleSegments) + 1);
    vertices.push_back(SDL_Vertex{{centerX, centerY}, color, {0.0f, 0.0f}});

    constexpr float twoPi = 2.0f * std::numbers::pi_v<float>;
    for (int i = 0; i < kCircleSegments; ++i) {
        const float angle = twoPi * static_cast<float>(i) / static_cast<float>(kCircleSegments);
        vertices.push_back(SDL_Vertex{{centerX + std::cos(angle) * radius,
                                       centerY + std::sin(angle) * radius},
                                      color, {0.0f, 0.0f}});
    }

    std::vector<int> indices;
    indices.reserve(static_cast<size_t>(kCircleSegments) * 3);
    for (int i = 0; i < kCircleSegments; ++i) {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 1 == kCircleSegments ? 1 : i + 2);
    }

    SDL_RenderGeometry(m_renderer, nullptr,
                       vertices.data(), static_cast<int>(vertices.size()),
                       indices.data(), static_cast<int>(indices.size()));
}

int main(int, char**) {
    SdlMain example;
    if (!example.init("Shimera - SDL3 2D example", kWindowWidth, kWindowHeight))
        return 1;

    example.run();
    return 0;
}
