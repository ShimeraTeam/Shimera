#include "RaylibMain.hpp"
#include <rlgl.h>

#include <stdexcept>
#include <string>
#include <iostream>

namespace {

constexpr int kWindowWidth = 960;
constexpr int kWindowHeight = 540;

}

RaylibMain::RaylibMain(const char* title, const int width, const int height) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(width, height, title);
    if (!IsWindowReady())
        throw std::runtime_error("InitWindow failed");

    SetTargetFPS(60);

    m_camera.position = Vector3{10.0f, 8.0f, 10.0f};
    m_camera.target = Vector3{0.0f, 1.0f, 0.0f};
    m_camera.up = Vector3{0.0f, 1.0f, 0.0f};
    m_camera.fovy = 45.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

void RaylibMain::InitShimera() {
    m_context.emplace(shimera::Context::create());
    std::cout << m_context->getRendererInfo() << std::endl;

    m_scene.emplace(kWindowWidth, kWindowHeight);
    m_fx.emplace(kWindowWidth, kWindowHeight);
    m_fx->add<shimera::HDRBloom>();
    m_fx->add<shimera::Distortion>();
}

RaylibMain::~RaylibMain() {

    if (IsWindowReady())
        CloseWindow();
}

void RaylibMain::run() {
    while (m_running && !WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE))
            m_running = false;

        update();
        draw();
    }
}

void RaylibMain::update() {
    UpdateCamera(&m_camera, CAMERA_ORBITAL);
}

void RaylibMain::draw() {
    m_scene->begin();
        ClearBackground(Color{18, 20, 28, 255});
        BeginMode3D(m_camera);

        drawCube(Vector3{-2.5f, 1.05f, 0.0f}, Vector3{2.8f, 2.8f, 2.8f}, Color{214, 79, 56, 255});
        drawSphere(Vector3{2.8f, 1.15f, 0.0f}, 1.15f, Color{41, 181, 242, 255});

        EndMode3D();
    m_scene->end();
    BeginDrawing();
        ClearBackground(BLACK);
        m_fx->get<shimera::Distortion>().withTime(GetTime());
        m_fx->render(*m_scene);
    EndDrawing();
}

void RaylibMain::drawCube(const Vector3 position, const Vector3 size, const Color color) const {
    DrawCube(position, size.x, size.y, size.z, color);
    DrawCubeWires(position, size.x, size.y, size.z, Color{255, 236, 214, 255});
}

void RaylibMain::drawSphere(const Vector3 position, const float radius, const Color color) const {
    DrawSphere(position, radius, color);
    DrawSphereWires(position, radius, 16, 16, Color{255, 236, 214, 120});
}

int main() {
    try {
        RaylibMain example("Shimera - Raylib 3D example", kWindowWidth, kWindowHeight);
        example.InitShimera();
        example.run();
    } catch (const std::exception& error) {
        TraceLog(LOG_ERROR, "%s", error.what());
        return 1;
    }

    return 0;
}