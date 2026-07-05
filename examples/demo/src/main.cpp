#include "raylib.h"
#include <GL/glew.h>
#include "rlgl.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <print>
#include <stdexcept>

#include "shimera.h"
#include "EffectPipeline.inl"
#include "backend/BackendFactory.hpp"
#include "effects/HDRBloomEffect.hpp"
#include "effects/ChromaticAberration.hpp"
#include "effects/VignetteEffect.hpp"
#include "effects/PixelisationEffect.hpp"
#include "effects/GaussianBlurEffect.hpp"
#include "effects/DistortionEffect.hpp"
#include "effects/ContrastEffect.hpp"
#include "effects/ColortintEffect.hpp"
#include "effects/AtmosphericScatteringEffect.hpp"
#include "effects/materials/FresnelEffect.hpp"
#include "backend/raylib/RaylibMesh.hpp"
#include "backend/raylib/converts/RaylibCamera.hpp"

namespace {

constexpr int kWidth = 1280;
constexpr int kHeight = 720;

constexpr float kHalf = 5.0f;
constexpr float kBoxHeight = 10.0f;
constexpr Vector3 kBoxCenter{0.0f, kBoxHeight * 0.45f, 0.0f};

constexpr Vector3 kSphereCenter{1.8f, 3.9f, 1.2f};
constexpr float kSphereRadius = 0.9f;
constexpr float kAtmosphereScale = 1.7f;

constexpr float kNearPlane = 0.1f;
constexpr float kFarPlane = 300.0f;

Vector3 add(Vector3 a, Vector3 b) { return Vector3{a.x + b.x, a.y + b.y, a.z + b.z}; }
Vector3 scale(Vector3 v, float s) { return Vector3{v.x * s, v.y * s, v.z * s}; }

shimera::Vec3<float> toVec3(Vector3 v) { return {v.x, v.y, v.z}; }

template<typename TEffect, typename Fn>
void updateIfActive(shimera::EffectPipeline &pipeline, Fn &&fn)
{
    try {
        fn(pipeline.get<TEffect>());
    } catch (const std::out_of_range &) {
        // Effect not in the pipeline this run, nothing to update
    }
}

void drawQuad(Vector3 a, Vector3 b, Vector3 c, Vector3 d, Color color)
{
    DrawTriangle3D(a, b, c, color);
    DrawTriangle3D(a, c, d, color);
}

void drawRotatedCube(Vector3 center, Vector3 size, float angleDeg, Color fill, Color wire)
{
    rlPushMatrix();
    rlTranslatef(center.x, center.y, center.z);
    rlRotatef(angleDeg, 0.0f, 1.0f, 0.0f);
    DrawCube(Vector3{0, 0, 0}, size.x, size.y, size.z, fill);
    DrawCubeWires(Vector3{0, 0, 0}, size.x, size.y, size.z, wire);
    rlPopMatrix();
}

void drawCornellBox()
{
    constexpr Color back{79, 79, 79, 255};
    constexpr Color floorCol{79, 79, 79, 255};
    constexpr Color ceilCol{79, 79, 79, 255};
    constexpr Color red{190, 45, 40, 255};
    constexpr Color green{45, 160, 60, 255};
    constexpr Color light{255, 255, 240, 255};

    constexpr float h = kHalf;
    constexpr float top = kBoxHeight;

    // floor (faces +y)
    drawQuad({-h, 0, -h}, {-h, 0, h}, {h, 0, h}, {h, 0, -h}, floorCol);
    // ceiling (faces -y)
    drawQuad({-h, top, -h}, {h, top, -h}, {h, top, h}, {-h, top, h}, ceilCol);
    // back wall (faces +z)
    drawQuad({-h, 0, -h}, {h, 0, -h}, {h, top, -h}, {-h, top, -h}, back);
    // left wall, red (faces +x)
    drawQuad({-h, 0, h}, {-h, 0, -h}, {-h, top, -h}, {-h, top, h}, red);
    // right wall, green (faces -x)
    drawQuad({h, 0, -h}, {h, 0, h}, {h, top, h}, {h, top, -h}, green);

    // ceiling area light (faces -y), bright enough for the bloom pass to pick up
    drawQuad({-2, top - 0.02f, -2}, {2, top - 0.02f, -2},
             {2, top - 0.02f, 2}, {-2, top - 0.02f, 2}, light);

    DrawCubeWires(Vector3{0.0f, top * 0.5f, 0.0f}, 2.0f * h, top, 2.0f * h,
                  Fade(RAYWHITE, 0.18f));
}

void drawScene()
{
    drawCornellBox();

    constexpr Color blockWire{100, 100, 110, 255};

    drawRotatedCube(Vector3{-1.8f, 3.0f, -1.5f}, Vector3{3.0f, 6.0f, 3.0f}, 18.0f, DARKBLUE, blockWire);
    drawRotatedCube(Vector3{1.8f, 1.5f, 1.2f}, Vector3{3.0f, 3.0f, 3.0f}, -15.0f, GRAY, blockWire);

    DrawSphereEx(kSphereCenter, kSphereRadius, 24, 24, Color{120, 180, 255, 255});
    DrawSphereWires(kSphereCenter, kSphereRadius + 0.01f, 12, 12, Color{170, 215, 255, 255});
}

void drawHud(const char *effects, bool freeCam)
{
    DrawRectangle(8, 8, 520, 132, Fade(BLACK, 0.55f));
    DrawRectangleLines(8, 8, 520, 132, Fade(RAYWHITE, 0.15f));
    DrawText("Shimera - Démonstration", 18, 16, 18, RAYWHITE);
    DrawText(TextFormat("Effets: %s", effects), 18, 44, 15, Color{120, 230, 255, 255});
    DrawText(TextFormat("Caméra: %s   [R] alterner", freeCam ? "Libre" : "Orbital"),
             18, 72, 14, Color{255, 210, 120, 255});
    if (freeCam) {
        DrawText("ZQSD bouger   Espace/Maj haut-bas", 18, 100, 14, GRAY);
    } else {
        DrawText("auto-orbital   molette = zoom", 18, 100, 14, GRAY);
    }
}

}

int main()
{
    InitWindow(kWidth, kHeight, "Shimera - Démonstration");

    if (glewInit() != GLEW_OK) {
        std::cerr << "[GLEW] initialization failed!" << '\n';
        return -1;
    }
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';

    rlSetClipPlanes(kNearPlane, kFarPlane);

    Camera camera = {};
    camera.position = Vector3{12.0f, 8.0f, 16.0f};
    camera.target = kBoxCenter;
    camera.up = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    bool freeCam = false;
    float yaw = 0.0f;
    float pitch = 0.0f;

    // 1. Pick a backend. The factory returns the one this target was built with.
    shimera::IBackend *backend = shimera::BackendFactory::create();
    if (!backend) {
        std::cerr << "Failed to create backend!" << '\n';
        CloseWindow();
        return -1;
    }

    // 2. Render the scene into an offscreen framebuffer (with a samplable depth
    // buffer, which the atmospheric scattering effect needs).
    shimera::IFrameBuffer *sceneFb = backend->createFrameBuffer(kWidth, kHeight, true);

    const auto resolution = shimera::Vec2(static_cast<float>(kWidth), static_cast<float>(kHeight));

    // 3. Create and configure the effects. Every parameter is set through a
    // fluent .withX() call. No shader code, no GPU knowledge required.
    shimera::HDRBloomEffect bloom(backend);
    bloom.withThreshold(0.95f)
         .withKnee(0.2f)
         .withIntensity(1.3f)
         .withBlurSigma(20.0f)
         .withBlurSamples(60)
         .withResolution(resolution);

    shimera::ChromaticAberrationEffect chroma(backend);
    chroma.withStrength(0.7f)
          .withRadius(true)
          .withContrast(2.0f)
          .withSamples(16);

    shimera::VignetteEffect vignette(backend);
    vignette.withStrength(1.2f)
            .withRadius(0.5f)
            .withGap(0.4f)
            .withIsRounded(true)
            .withResolution(resolution);

    shimera::PixelisationEffect pixelise(backend);
    pixelise.withPixelSizeX(4.0f)
            .withPixelSizeY(4.0f)
            .withResolution(resolution);

    shimera::GaussianBlurEffect blur(backend);
    blur.withSigma(6.0f)
         .withSamples(20)
         .withResolution(resolution);

    shimera::DistortionEffect distortion(backend);
    distortion.withNoiseScale(2.0f)
              .withDistortionStrength(0.04f)
              .withTimeScale(0.3f);

    shimera::ContrastEffect contrast(backend);
    contrast.withContrastFactor(1.6f);

    shimera::ColortintEffect colortint(backend);
    colortint.withTint(shimera::Vec3(1.0f, 0.5f, 0.15f));

    shimera::AtmosphericScatteringEffect atmosphere(backend);
    atmosphere.withSun(shimera::Vec3(100.0f, 100.0f, 0.0f))
              .withQuality(16, 16);

    // A 3D material effect.
    Model fresnelModel = LoadModelFromMesh(GenMeshSphere(1.1f, 32, 32));
    shimera::RaylibMesh fresnelMesh(fresnelModel);

    shimera::FresnelEffect fresnel(backend);
    fresnel.withColor(shimera::Vec3(0.3f, 0.7f, 1.0f))
           .withPower(3.0f)
           .withReflectance(0.04f)
           .withIntensity(1.5f);
    fresnel.setTransform(shimera::Vec3(-1.8f, 7.1f, -1.5f));

    // 4. Assemble the post-processing chain.
    shimera::EffectPipeline pipeline(backend, kWidth, kHeight);
    // pipeline.addEffect(std::move(bloom));        // HDR bloom: glow on bright areas
    // pipeline.addEffect(std::move(chroma));       // chromatic aberration
    // pipeline.addEffect(std::move(vignette));     // darkened edges
    // pipeline.addEffect(std::move(pixelise));     // retro pixelation
    // pipeline.addEffect(std::move(blur));         // gaussian blur
    // pipeline.addEffect(std::move(distortion));   // animated screen distortion
    // pipeline.addEffect(std::move(contrast));     // contrast boost
    // pipeline.addEffect(std::move(colortint));    // warm color tint
    // pipeline.addEffect(std::move(atmosphere));   // atmospheric scattering on the sphere

    // Seulement quand la pipeline est vide
    shimera::IPostProcessor *passthrough = backend->createPostProcessor(
        "../../../../res/shader/postprocessing/postprocess.vert",
        "../../../../res/shader/postprocessing/normal.frag");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        const auto t = static_cast<float>(GetTime());

        if (IsKeyPressed(KEY_R)) {
            freeCam = !freeCam;
            if (freeCam) {
                DisableCursor();
                const Vector3 d = add(camera.target, scale(camera.position, -1.0f));
                const float len = std::sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
                yaw = std::atan2(d.x, d.z);
                pitch = std::asin(d.y / len);
            } else {
                EnableCursor();
                camera.target = kBoxCenter;
            }
        }

        if (freeCam) {
            const auto [x, y] = GetMouseDelta();
            yaw -= x * 0.003f;
            pitch = std::clamp(pitch - y * 0.003f, -1.5f, 1.5f);

            const Vector3 forward{std::cos(pitch) * std::sin(yaw), std::sin(pitch),
                                  std::cos(pitch) * std::cos(yaw)};
            const float flatLen = std::max(std::sqrt(forward.x * forward.x + forward.z * forward.z), 0.001f);
            const Vector3 right{-forward.z / flatLen, 0.0f, forward.x / flatLen};

            Vector3 move{0, 0, 0};
            if (IsKeyDown(KEY_W)) move = add(move, forward);
            if (IsKeyDown(KEY_S)) move = add(move, scale(forward, -1.0f));
            if (IsKeyDown(KEY_A)) move = add(move, scale(right, -1.0f));
            if (IsKeyDown(KEY_D)) move = add(move, right);
            if (IsKeyDown(KEY_SPACE)) move.y += 1.0f;
            if (IsKeyDown(KEY_LEFT_SHIFT)) move.y -= 1.0f;

            camera.position = add(camera.position, scale(move, 8.0f * GetFrameTime()));
            camera.target = add(camera.position, forward);
        } else {
            UpdateCamera(&camera, CAMERA_ORBITAL);
        }

        // shimera camera for the 3D material pass
        const shimera::Camera shCam = shimera::RaylibCamera::toShimera(camera, kNearPlane, kFarPlane);

        sceneFb->bind();
        sceneFb->clear(shimera::Color{0.03f, 0.04f, 0.06f, 1.0f});
        BeginMode3D(camera);
            drawScene();
            // fresnel.render(fresnelMesh, shCam);
        EndMode3D();
        sceneFb->unbind();

        updateIfActive<shimera::DistortionEffect>(pipeline, [t](auto &fx) {
            fx.withTime(t);
        });
        updateIfActive<shimera::AtmosphericScatteringEffect>(pipeline, [&](auto &fx) {
            const float aspect = static_cast<float>(kWidth) / static_cast<float>(kHeight);
            fx.withCamera(toVec3(camera.position), toVec3(camera.target),
                          camera.fovy, aspect, kNearPlane, kFarPlane)
              .withPlanet(toVec3(kSphereCenter), kSphereRadius,
                          kSphereRadius * kAtmosphereScale);
        });

        BeginDrawing();
            ClearBackground(BLACK);
            if (pipeline.size() == 0) {
                passthrough->render(sceneFb->getTexture());
            } else {
                pipeline.render(sceneFb->getTexture(), &sceneFb->getDepthTexture());
            }
            const std::string effects = pipeline.getEffectsNames();
            drawHud(effects.c_str(), freeCam);
        EndDrawing();
    }

    UnloadModel(fresnelModel);
    delete passthrough;
    delete sceneFb;
    delete backend;
    CloseWindow();
    exit(EXIT_SUCCESS);
    return 0;
}
