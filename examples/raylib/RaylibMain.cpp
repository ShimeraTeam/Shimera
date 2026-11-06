// SPDX-License-Identifier: GPL-3.0-only
//
// Shimera: a simple way to add visual effects without any GPU knowledge
// Copyright (C) 2025-2026 The Shimera Authors
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "RaylibMain.hpp"

#include <raymath.h>
#include <rlgl.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace {

constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;

constexpr float kPlanetRadius = 500.0f;
constexpr float kAtmosphereHeight = 52.0f;
constexpr float kHeightScale = 3.0f;
const Color kGroundColor{46, 120, 58, 255};

constexpr float kSunIntensity = 2.0f;

constexpr double kSkyUnitsPerKm = 1000.0;
constexpr float kSkyHazeScale = 20.0f;
constexpr float kSkyGroundSize = 16000.0f;

constexpr float kMouseSensitivity = 0.12f; // degrees per pixel
constexpr float kMinHeight = 0.5f; // the camera stays this far above the ground

// The two props, in both worlds.
const Color kPillarColor{214, 79, 56, 255};
const Color kBallColor{41, 181, 242, 255};
const Color kPropEdges{255, 236, 214, 255};
constexpr float kPropSink = 0.5f; // into the planet, so its flat triangles never leave them floating

void printHelp() {
    std::cout <<
        "Shimera - Raylib atmosphere example\n"
        "  Right mouse + move   look around\n"
        "  W/A/S/D, Q/E         move (speed follows the height), Shift = faster\n"
        "  I/K, J/L             sun elevation / azimuth      T  toggle sun motion\n"
        "  M                    switch planet / flat ground (sky mode)\n"
        "  Esc                  quit\n";
}

shimera::Vec3<float> toShimera(const Vector3 v) {
    return {v.x, v.y, v.z};
}

} // namespace

RaylibMain::RaylibMain(const char* title, const int width, const int height) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, title);
    if (!IsWindowReady())
        throw std::runtime_error("InitWindow failed");

    SetTargetFPS(60);

    m_camera.fovy = 60.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

void RaylibMain::InitShimera() {
    m_context.emplace(shimera::Context::create());
    std::cout << m_context->getRendererInfo() << std::endl;

    // The atmosphere reads the scene depth, hence the samplable depth texture.
    m_scene.emplace(GetScreenWidth(), GetScreenHeight(), true);
    m_fx.emplace(GetScreenWidth(), GetScreenHeight(), /*hdr=*/true);
    m_fx->add<shimera::AtmosphericScattering>();

    auto& sky = m_fx->get<shimera::AtmosphericScattering>();
    shimera::AtmosphereProfile profile = shimera::AtmospherePresets::earth();
    profile.densityScale = 3.0f;
    sky.withProfile(profile)
       .withSunIlluminance({kSunIntensity, kSunIntensity, kSunIntensity})
       .withSceneLinear(false); // raylib draws plain 8-bit colours
    applyMode();

    printHelp();
}

RaylibMain::~RaylibMain() {
    m_fx.reset();
    m_scene.reset();
    m_context.reset();
    if (IsWindowReady())
        CloseWindow();
}

// The planet and The flat ground mode switch
void RaylibMain::applyMode() {
    auto& sky = m_fx->get<shimera::AtmosphericScattering>();
    if (m_skyMode) {
        sky.withSkyMode(0.0, kSkyUnitsPerKm).withAerialDistanceScale(kSkyHazeScale);
        sky.profile().heightScale = 1.0f;
        m_camera.position = Vector3{0.0f, 2.0f, 30.0f};
        m_camera.target = Vector3{0.0f, 5.0f, -100.0f};
    } else {
        sky.withPlanet({0.0, 0.0, 0.0}, kPlanetRadius, kAtmosphereHeight).withAerialDistanceScale(1.0f);
        sky.profile().heightScale = kHeightScale;
        m_camera.position = Vector3{0.0f, 1.2f * kPlanetRadius, 2.6f * kPlanetRadius};
        m_camera.target = Vector3{0.0f, 0.0f, 0.0f};
    }
    m_camera.up = Vector3{0.0f, 1.0f, 0.0f};
}

void RaylibMain::run() {
    while (m_running && !WindowShouldClose()) {
        if (IsWindowResized()) {
            m_scene->resize(GetScreenWidth(), GetScreenHeight());
            m_fx->resize(GetScreenWidth(), GetScreenHeight());
        }
        update(GetFrameTime());
        draw();
    }
}

void RaylibMain::update(const float dt) {
    if (IsKeyPressed(KEY_ESCAPE))
        m_running = false;
    if (IsKeyPressed(KEY_M)) {
        m_skyMode = !m_skyMode;
        applyMode();
    }
    updateCamera(dt);
    updateSun(dt);
}

// The sun by angles, if not using position. Automatic motion is a sunset on a loop.
void RaylibMain::updateSun(const float dt) {
    if (IsKeyPressed(KEY_T))
        m_sunMoves = !m_sunMoves;

    const float rate = dt * 20.0f;
    if (IsKeyDown(KEY_I)) m_sunElevation += rate;
    if (IsKeyDown(KEY_K)) m_sunElevation -= rate;
    if (IsKeyDown(KEY_J)) m_sunAzimuth -= rate;
    if (IsKeyDown(KEY_L)) m_sunAzimuth += rate;
    if (m_sunMoves) {
        m_sunElevation -= dt * 2.0f;
        if (m_sunElevation < -12.0f)
            m_sunElevation = 60.0f;
    }
    m_sunElevation = std::clamp(m_sunElevation, -90.0f, 90.0f);
    m_sunAzimuth = std::fmod(m_sunAzimuth + 360.0f, 360.0f);
}

/* A free camera, right mouse to look, WASD to fly, Q/E down and up.
 * Speed scale the more close to the ground */
void RaylibMain::updateCamera(const float dt) {
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        const Vector2 delta = GetMouseDelta();
        const Vector3 forward = Vector3Normalize(Vector3Subtract(m_camera.target, m_camera.position));
        const Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, m_camera.up));
        Vector3 dir = Vector3RotateByAxisAngle(forward, m_camera.up, -delta.x * kMouseSensitivity * DEG2RAD);
        dir = Vector3RotateByAxisAngle(dir, right, -delta.y * kMouseSensitivity * DEG2RAD);
        m_camera.target = Vector3Add(m_camera.position, dir);
    }

    const float height = m_skyMode ? m_camera.position.y : Vector3Length(m_camera.position) - kPlanetRadius;
    float speed = m_skyMode ? std::max(height, 5.0f) * 2.0f : std::max(height, 0.5f) * 0.8f;
    if (IsKeyDown(KEY_LEFT_SHIFT))
        speed *= 4.0f;

    const Vector3 forward = Vector3Normalize(Vector3Subtract(m_camera.target, m_camera.position));
    const Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, m_camera.up));
    Vector3 move{0, 0, 0};
    if (IsKeyDown(KEY_W)) move = Vector3Add(move, forward);
    if (IsKeyDown(KEY_S)) move = Vector3Subtract(move, forward);
    if (IsKeyDown(KEY_D)) move = Vector3Add(move, right);
    if (IsKeyDown(KEY_A)) move = Vector3Subtract(move, right);
    if (IsKeyDown(KEY_E)) move = Vector3Add(move, m_camera.up);
    if (IsKeyDown(KEY_Q)) move = Vector3Subtract(move, m_camera.up);
    if (Vector3Length(move) > 0.0f) {
        move = Vector3Scale(Vector3Normalize(move), speed * dt);
        m_camera.position = Vector3Add(m_camera.position, move);
        m_camera.target = Vector3Add(m_camera.target, move);
    }

    // Stay above the ground.
    Vector3 lifted = m_camera.position;
    if (m_skyMode)
        lifted.y = std::max(lifted.y, kMinHeight);
    else if (Vector3Length(lifted) < kPlanetRadius + kMinHeight)
        lifted = Vector3Scale(Vector3Normalize(lifted), kPlanetRadius + kMinHeight);
    m_camera.target = Vector3Add(m_camera.target, Vector3Subtract(lifted, m_camera.position));
    m_camera.position = lifted;

    // "Up" is away from the ground, so the horizon stays level wherever we are.
    const Vector3 radial = m_skyMode ? Vector3{0, 1, 0} : Vector3Normalize(m_camera.position);
    const Vector3 view = Vector3Normalize(Vector3Subtract(m_camera.target, m_camera.position));
    Vector3 up = Vector3Subtract(radial, Vector3Scale(view, Vector3DotProduct(radial, view)));
    if (Vector3Length(up) < 1e-3f)
        up = Vector3CrossProduct(view, std::fabs(view.x) < 0.9f ? Vector3{1, 0, 0} : Vector3{0, 0, 1});
    m_camera.up = Vector3Normalize(up);
}

// The planet with a pillar and a ball side by side.
void RaylibMain::drawPlanet() const {
    DrawSphereEx(Vector3{0, 0, 0}, kPlanetRadius, 96, 96, kGroundColor);

    // Rotate +y onto the local vertical, then move out to the ground (sunk a little so the
    // flat triangles of the planet mesh never leave the props floating).
    const Vector3 up = Vector3Normalize(Vector3{0.0f, 0.45f, 1.0f});
    const Vector3 axis = Vector3Normalize(Vector3CrossProduct(Vector3{0, 1, 0}, up));
    rlPushMatrix();
        rlRotatef(std::acos(up.y) * RAD2DEG, axis.x, axis.y, axis.z);
        rlTranslatef(0.0f, kPlanetRadius - kPropSink, 0.0f);
        DrawCubeV(Vector3{-6.0f, 8.0f, 0.0f}, Vector3{4.0f, 16.0f, 4.0f}, kPillarColor);
        DrawCubeWiresV(Vector3{-6.0f, 8.0f, 0.0f}, Vector3{4.0f, 16.0f, 4.0f}, kPropEdges);
        DrawSphereEx(Vector3{6.0f, 5.0f, 0.0f}, 5.0f, 24, 24, kBallColor);
    rlPopMatrix();
}

// Sky mode's world, in metres. Flat ground, a pillar and a ball ahead of the starting camera.
void RaylibMain::drawFlatWorld() const {
    DrawPlane(Vector3{0, 0, 0}, Vector2{kSkyGroundSize, kSkyGroundSize}, kGroundColor);
    DrawCubeV(Vector3{-8.0f, 6.0f, -10.0f}, Vector3{3.0f, 12.0f, 3.0f}, kPillarColor);
    DrawCubeWiresV(Vector3{-8.0f, 6.0f, -10.0f}, Vector3{3.0f, 12.0f, 3.0f}, kPropEdges);
    DrawSphereEx(Vector3{8.0f, 4.0f, -25.0f}, 4.0f, 24, 24, kBallColor);
}

void RaylibMain::draw() {
    // raylib's default clip planes (0.01 .. 1000) are too short for either world.
    rlSetClipPlanes(0.1, m_skyMode ? 12000.0 : 5000.0);

    m_scene->begin();
        ClearBackground(BLACK);
        BeginMode3D(m_camera);
            if (m_skyMode) {
                drawFlatWorld();
            } else {
                drawPlanet();
            }
            // Matrices exactly as raylib set them up, read while the 3D mode is active.
            const shimera::Camera camera = shimera::RaylibTarget::cameraFromRaylib(toShimera(m_camera.position));
        EndMode3D();
    m_scene->end();

    m_fx->get<shimera::AtmosphericScattering>()
        .withCamera(camera)
        .withSunAngles(m_sunElevation, m_sunAzimuth);

    BeginDrawing();
        ClearBackground(BLACK);
        m_fx->render(*m_scene);
    EndDrawing();
}

int main() {
    try {
        RaylibMain example("Shimera - Raylib atmosphere example", kWindowWidth, kWindowHeight);
        example.InitShimera();
        example.run();
    } catch (const std::exception& error) {
        TraceLog(LOG_ERROR, "%s", error.what());
        return 1;
    }
    return 0;
}
