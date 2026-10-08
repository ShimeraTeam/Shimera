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

#include "Context.hpp"
#include "EffectPipeline.hpp"
#include "hosts/SdlTarget.hpp"

#include <SDL3/SDL.h>

#include <optional>

/* A standalone SDL3 example */
class SdlMain {
public:
    SdlMain() = default;
    ~SdlMain();

    SdlMain(const SdlMain&) = delete;
    SdlMain& operator=(const SdlMain&) = delete;


    bool init(const char* title, int width, int height);

    void run();

private:
    bool loadBackground(const char* path);
    void resize(int width, int height);

    void draw();
    void drawBackground() const;
    void drawRectangle(const SDL_FRect& rect) const;
    void drawCircle(float centerX, float centerY, float radius, SDL_FColor color) const;

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture* m_background = nullptr;
    bool m_running = true;

    std::optional<shimera::Context> m_context;
    std::optional<shimera::SdlTarget> m_scene;
    std::optional<shimera::EffectPipeline> m_fx;
};
