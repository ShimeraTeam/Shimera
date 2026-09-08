#include "raylib.h"
#include <GL/glew.h>
#include "rlgl.h"

#include <iostream>
#include <string>

#include "backend/BackendFactory.hpp"
#include "backend/raylib/converts/RaylibCamera.hpp"

#include "CameraController.hpp"
#include "DemoConfig.hpp"
#include "Hud.hpp"
#include "Scene.hpp"
#include "ShaderMenu.hpp"
#include "ShaderShowcase.hpp"

namespace {

// Main loop: input, scene into the offscreen buffer, then post-processing.
void runDemo(shimera::IBackend *backend, shimera::IFrameBuffer &sceneFb)
{
    demo::ShaderShowcase showcase(backend);
    demo::CameraController camera;
    demo::ShaderMenu menu;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        const auto t = static_cast<float>(GetTime());

        bool menuHovered = false;
        if constexpr (demo::kUseShaderMenu) {
            demo::updateShaderMenu(menu, showcase.toggles(), !camera.isFreeCam());
            menuHovered = demo::isMouseOverMenu(menu, showcase.toggles());
            showcase.applyToggles();
        }

        camera.update(menuHovered);

        // shimera camera for the 3D material pass
        const shimera::Camera shCam =
            shimera::RaylibCamera::toShimera(camera.camera(), demo::kNearPlane, demo::kFarPlane);

        sceneFb.bind();
        sceneFb.clear(shimera::Color{0.03f, 0.04f, 0.06f, 1.0f});
        BeginMode3D(camera.camera());
            demo::drawScene();
            showcase.renderMaterials(shCam);
        EndMode3D();
        sceneFb.unbind();

        showcase.updateUniforms(t, camera.camera());

        const std::string effects = showcase.activeEffectNames();

        BeginDrawing();
            ClearBackground(BLACK);
            showcase.render(sceneFb);
            demo::drawHud(effects.c_str(), camera.isFreeCam(), demo::kUseShaderMenu);
            if constexpr (demo::kUseShaderMenu)
                demo::drawShaderMenu(menu, showcase.toggles(), !camera.isFreeCam());
        EndDrawing();
    }
}

}

int main()
{
    InitWindow(demo::kWidth, demo::kHeight, "Shimera - Démonstration");

    if (glewInit() != GLEW_OK) {
        std::cerr << "[GLEW] initialization failed!" << '\n';
        return -1;
    }
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';

    rlSetClipPlanes(demo::kNearPlane, demo::kFarPlane);

    // 1. Pick a backend. The factory returns the one this target was built with.
    shimera::IBackend *backend = shimera::BackendFactory::create();
    if (!backend) {
        std::cerr << "Failed to create backend!" << '\n';
        CloseWindow();
        return -1;
    }

    // 2. Render the scene into an offscreen framebuffer (with a samplable depth
    // buffer, which the atmospheric scattering effect needs).
    shimera::IFrameBuffer *sceneFb = backend->createFrameBuffer(demo::kWidth, demo::kHeight, true);

    // 3. Effects, pipeline and main loop. Scoped so that everything holding a
    // GPU resource is released before the window goes away.
    runDemo(backend, *sceneFb);

    delete sceneFb;
    delete backend;
    CloseWindow();
    exit(EXIT_SUCCESS);
    return 0;
}
