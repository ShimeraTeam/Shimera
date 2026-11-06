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

#pragma once

#include <raylib.h>

#include <shimera.hpp>
#include <hosts/RaylibTarget.hpp>

#include <optional>

/**
 * A standalone Raylib 3D example. Shimera's atmosphere over a planet with a box and a
 * sphere on it, or over a flat ground (sky mode), with a free camera. Keys are printed
 * at start.
 */
class RaylibMain {
public:
    RaylibMain(const char* title, int width, int height);
    ~RaylibMain();

    RaylibMain(const RaylibMain&) = delete;
    RaylibMain& operator=(const RaylibMain&) = delete;
    void InitShimera();
    void run();

private:
    void drawPlanet() const;
    void drawFlatWorld() const;
    void applyMode();
    void update(float dt);
    void updateCamera(float dt);
    void updateSun(float dt);
    void draw();

    Camera3D m_camera{};
    float m_sunElevation = 30.0f; // degrees above the horizon
    float m_sunAzimuth = 150.0f; // degrees, compass: 0 north (-Z), 90 east (+X)
    bool m_sunMoves = true;
    bool m_skyMode = false;
    bool m_running = true;

    std::optional<shimera::Context> m_context;
    std::optional<shimera::RaylibTarget> m_scene;
    std::optional<shimera::EffectPipeline> m_fx;
};
