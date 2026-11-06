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

#include <gtest/gtest.h>

#include <memory>
#include <optional>

#include "shimera.hpp"

#if defined(SHIMERA_HOST_GLFW)
    #include <GLFW/glfw3.h>
    #include "hosts/GlfwTarget.hpp"
#elif defined(SHIMERA_HOST_SFML)
    #include <SFML/Graphics/RenderWindow.hpp>
    #include "hosts/SfmlTarget.hpp"
#elif defined(SHIMERA_HOST_RAYLIB)
    #include <raylib.h>
    #include "hosts/RaylibTarget.hpp"
#elif defined(SHIMERA_HOST_SDL)
    #include <SDL3/SDL.h>
    #include "hosts/SdlTarget.hpp"
#else
    #error "Define the host under test: SHIMERA_HOST_GLFW, _SFML, _RAYLIB or _SDL"
#endif


class ContextTest : public ::testing::Test {
    protected:
        static constexpr int kWidth = 640;
        static constexpr int kHeight = 480;

        static void SetUpTestSuite() {
#if defined(SHIMERA_HOST_GLFW)
            if (!glfwInit())
                GTEST_FAIL() << "glfwInit failed";
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
            s_window = glfwCreateWindow(kWidth, kHeight, "GLFW window", nullptr, nullptr);
            if (s_window == nullptr)
                GTEST_FAIL() << "glfwCreateWindow failed";
            glfwMakeContextCurrent(s_window);
#elif defined(SHIMERA_HOST_SFML)
            s_window = std::make_unique<sf::RenderWindow>(sf::VideoMode({kWidth, kHeight}),
                                                          "SFML window", sf::Style::None);
            s_window->setVisible(false);
            if (!s_window->setActive(true))
                GTEST_FAIL() << "could not activate the SFML window";
#elif defined(SHIMERA_HOST_RAYLIB)
            SetConfigFlags(FLAG_WINDOW_HIDDEN);
            InitWindow(kWidth, kHeight, "Raylib window");
            if (!IsWindowReady())
                GTEST_FAIL() << "InitWindow failed";
#elif defined(SHIMERA_HOST_SDL)
            if (!SDL_Init(SDL_INIT_VIDEO))
                GTEST_FAIL() << "SDL_Init failed: " << SDL_GetError();
            s_window = SDL_CreateWindow("SDL window", kWidth, kHeight, SDL_WINDOW_HIDDEN);
            if (s_window == nullptr)
                GTEST_FAIL() << "SDL_CreateWindow failed: " << SDL_GetError();
            s_renderer = SDL_CreateRenderer(s_window, "opengl");
            if (s_renderer == nullptr)
                GTEST_FAIL() << "SDL_CreateRenderer(opengl) failed: " << SDL_GetError();
#endif
            s_context.emplace(shimera::Context::create());
        }

        static void TearDownTestSuite() {
            s_context.reset();
#if defined(SHIMERA_HOST_GLFW)
            if (s_window != nullptr)
                glfwDestroyWindow(s_window);
            s_window = nullptr;
            glfwTerminate();
#elif defined(SHIMERA_HOST_SFML)
            s_window.reset();
#elif defined(SHIMERA_HOST_RAYLIB)
            if (IsWindowReady())
                CloseWindow();
#elif defined(SHIMERA_HOST_SDL)
            if (s_renderer != nullptr)
                SDL_DestroyRenderer(s_renderer);
            s_renderer = nullptr;
            if (s_window != nullptr)
                SDL_DestroyWindow(s_window);
            s_window = nullptr;
            SDL_Quit();
#endif
        }

        static std::unique_ptr<shimera::IHostTarget> makeHostTarget(const int width, const int height) {
#if defined(SHIMERA_HOST_GLFW)
            return std::make_unique<shimera::GlfwTarget>(width, height);
#elif defined(SHIMERA_HOST_SFML)
            return std::make_unique<shimera::SfmlTarget>(*s_window, width, height);
#elif defined(SHIMERA_HOST_RAYLIB)
            return std::make_unique<shimera::RaylibTarget>(width, height);
#elif defined(SHIMERA_HOST_SDL)
            return std::make_unique<shimera::SdlTarget>(*s_renderer, width, height);
#endif
        }

    private:
        static inline std::optional<shimera::Context> s_context;
#if defined(SHIMERA_HOST_GLFW)
        static inline GLFWwindow* s_window = nullptr;
#elif defined(SHIMERA_HOST_SFML)
        static inline std::unique_ptr<sf::RenderWindow> s_window;
#elif defined(SHIMERA_HOST_SDL)
        static inline SDL_Window* s_window = nullptr;
        static inline SDL_Renderer* s_renderer = nullptr;
#endif
};
