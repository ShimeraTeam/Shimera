#pragma once

#include <raylib.h>

#include <shimera.hpp>
#include <hosts/RaylibTarget.hpp>

/* A standalone Raylib 3D example */
class RaylibMain {
public:
    RaylibMain(const char* title, int width, int height);
    ~RaylibMain();

    RaylibMain(const RaylibMain&) = delete;
    RaylibMain& operator=(const RaylibMain&) = delete;
    void InitShimera();
    void run();

private:
    void update();
    void draw();
    void drawCube(Vector3 position, Vector3 size, Color color) const;
    void drawSphere(Vector3 position, float radius, Color color) const;

    Camera3D m_camera{};
    bool m_running = true;
    std::optional<shimera::Context> m_context;
    std::optional<shimera::RaylibTarget> m_scene;
    std::optional<shimera::EffectPipeline> m_fx;
};