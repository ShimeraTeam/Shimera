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