#include "raylib.h"
#include <GL/glew.h>
#include "rlgl.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <numbers>

#include <shimera.h>
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

namespace {

constexpr int kWidth = 1280;
constexpr int kHeight = 720;

enum class Shape : unsigned char { Cube, Sphere, Cylinder, Cone, Pyramid, Capsule, HexPrism, Diamond };

struct ShapeEntry {
    Shape shape;
    Color color;
    float spin;
    float emission;
};

constexpr std::array<ShapeEntry, 8> kShapes = {{
    {Shape::Cube,     Color{230,  60,  60, 255}, 24.0f, 0.0f},  // red,    matte
    {Shape::Sphere,   Color{ 70, 150, 255, 255}, 18.0f, 1.0f},  // blue,   glowing
    {Shape::Cylinder, Color{ 90, 220, 110, 255}, 30.0f, 0.25f}, // green,  faint
    {Shape::Cone,     Color{255, 160,  40, 255}, 22.0f, 0.6f},  // orange, lit
    {Shape::Pyramid,  Color{180, 100, 255, 255}, 28.0f, 0.0f},  // violet, matte
    {Shape::Capsule,  Color{ 40, 225, 225, 255}, 20.0f, 0.85f}, // cyan,   glowing
    {Shape::HexPrism, Color{255, 215,  60, 255}, 26.0f, 0.4f},  // gold,   lit
    {Shape::Diamond,  Color{255, 110, 200, 255}, 32.0f, 0.0f},  // pink,   matte
}};

constexpr float kRingRadius = 9.0f;       // radius the ring shapes sit on
constexpr float kAtmosphereScale = 1.7f;  // atmosphere radius / planet radius

Vector3 add(Vector3 a, Vector3 b) { return Vector3{a.x + b.x, a.y + b.y, a.z + b.z}; }

shimera::Vec3<float> toVec3(Vector3 v) { return {v.x, v.y, v.z}; }

// World-space center & radius of the existing ring sphere (kShapes index 1),
// so the atmospheric scattering effect can be pointed straight at it.
constexpr int kSphereShapeIndex = 1;
constexpr float kSphereRadius = 1.3f;
Vector3 ringSphereCenter()
{
    const float a = (static_cast<float>(kSphereShapeIndex) / kShapes.size()) * 2.0f * std::numbers::pi_v<float>;
    return Vector3{std::cos(a) * kRingRadius, 1.55f, std::sin(a) * kRingRadius};
}

// Slightly darker shade, used for the solid fill so the wireframe pops on top.
Color shade(Color c, float k)
{
    const auto m = [&](unsigned char v) {
        const auto scaled = static_cast<int>(static_cast<float>(v) * k);
        return static_cast<unsigned char>(std::clamp(scaled, 0, 255));
    };
    return Color{m(c.r), m(c.g), m(c.b), c.a};
}

Color withEmission(Color base, float e)
{
    return shade(base, 0.5f + e * 1.7f); // e=0 -> x0.5 (dim), e=1 -> x2.2 (glow)
}


void drawShape(Shape shape, Vector3 base, float angle, Color baseColor, float emission)
{
    const Color color = withEmission(baseColor, emission);

    DrawCylinderEx(base, add(base, Vector3{0, 0.25f, 0}), 1.7f, 1.7f, 24, Color{30, 32, 40, 255});
    DrawCylinderWiresEx(base, add(base, Vector3{0, 0.25f, 0}), 1.7f, 1.7f, 24, Color{70, 74, 88, 255});

    const Color fill = shade(color, 0.85f);

    rlPushMatrix();
    rlTranslatef(base.x, base.y + 0.25f, base.z);
    rlRotatef(angle, 0.0f, 1.0f, 0.0f);

    switch (shape) {
        case Shape::Cube:
            DrawCube(Vector3{0, 1.1f, 0}, 2.0f, 2.0f, 2.0f, fill);
            DrawCubeWires(Vector3{0, 1.1f, 0}, 2.0f, 2.0f, 2.0f, color);
            break;
        case Shape::Sphere:
            DrawSphereEx(Vector3{0, 1.3f, 0}, 1.3f, 18, 18, fill);
            DrawSphereWires(Vector3{0, 1.3f, 0}, 1.32f, 10, 10, color);
            break;
        case Shape::Cylinder:
            DrawCylinderEx(Vector3{0, 0, 0}, Vector3{0, 2.6f, 0}, 1.0f, 1.0f, 32, fill);
            DrawCylinderWiresEx(Vector3{0, 0, 0}, Vector3{0, 2.6f, 0}, 1.0f, 1.0f, 32, color);
            break;
        case Shape::Cone:
            DrawCylinderEx(Vector3{0, 0, 0}, Vector3{0, 2.8f, 0}, 1.2f, 0.0f, 32, fill);
            DrawCylinderWiresEx(Vector3{0, 0, 0}, Vector3{0, 2.8f, 0}, 1.2f, 0.0f, 32, color);
            break;
        case Shape::Pyramid:
            DrawCylinderEx(Vector3{0, 0, 0}, Vector3{0, 2.6f, 0}, 1.5f, 0.0f, 4, fill);
            DrawCylinderWiresEx(Vector3{0, 0, 0}, Vector3{0, 2.6f, 0}, 1.5f, 0.0f, 4, color);
            break;
        case Shape::Capsule:
            DrawCapsule(Vector3{0, 0.8f, 0}, Vector3{0, 2.4f, 0}, 0.8f, 16, 12, fill);
            DrawCapsuleWires(Vector3{0, 0.8f, 0}, Vector3{0, 2.4f, 0}, 0.8f, 12, 8, color);
            break;
        case Shape::HexPrism:
            DrawCylinderEx(Vector3{0, 0, 0}, Vector3{0, 2.4f, 0}, 1.2f, 1.2f, 6, fill);
            DrawCylinderWiresEx(Vector3{0, 0, 0}, Vector3{0, 2.4f, 0}, 1.2f, 1.2f, 6, color);
            break;
        case Shape::Diamond:
            DrawCylinderEx(Vector3{0, 1.4f, 0}, Vector3{0, 2.8f, 0}, 1.3f, 0.0f, 4, fill);
            DrawCylinderEx(Vector3{0, 1.4f, 0}, Vector3{0, 0.0f, 0}, 1.3f, 0.0f, 4, fill);
            DrawCylinderWiresEx(Vector3{0, 1.4f, 0}, Vector3{0, 2.8f, 0}, 1.3f, 0.0f, 4, color);
            DrawCylinderWiresEx(Vector3{0, 1.4f, 0}, Vector3{0, 0.0f, 0}, 1.3f, 0.0f, 4, color);
            break;
    }
    rlPopMatrix();
}

void drawScene(float t)
{
    constexpr float ringRadius = kRingRadius;
    for (std::size_t i = 0; i < kShapes.size(); ++i) {
        const float a = (static_cast<float>(i) / kShapes.size()) * 2.0f * std::numbers::pi_v<float>;
        const Vector3 base{std::cos(a) * ringRadius, 0.0f, std::sin(a) * ringRadius};
        drawShape(kShapes[i].shape, base, t * kShapes[i].spin, kShapes[i].color, kShapes[i].emission);
    }

    constexpr float heroEmission = 0.95f;
    const Color gem = withEmission(Color{150, 110, 255, 255}, heroEmission);
    rlPushMatrix();
    rlTranslatef(0.0f, 2.6f, 0.0f);
    rlRotatef(t * 12.0f, 0.0f, 1.0f, 0.0f);
    DrawCylinderEx(Vector3{0, 0.0f, 0}, Vector3{0, 1.8f, 0}, 2.0f, 0.0f, 6, gem);
    DrawCylinderEx(Vector3{0, 0.0f, 0}, Vector3{0, -1.8f, 0}, 2.0f, 0.0f, 6, gem);
    DrawCylinderWiresEx(Vector3{0, 0.0f, 0}, Vector3{0, 1.8f, 0}, 2.0f, 0.0f, 6, Color{220, 200, 255, 255});
    DrawCylinderWiresEx(Vector3{0, 0.0f, 0}, Vector3{0, -1.8f, 0}, 2.0f, 0.0f, 6, Color{220, 200, 255, 255});
    rlPopMatrix();
    DrawSphereEx(Vector3{0, 2.6f, 0}, 0.5f, 16, 16, WHITE);
}

void drawHud(const char *effectName)
{
    DrawRectangle(8, 8, 450, 190, Fade(BLACK, 0.55f));
    DrawRectangleLines(8, 8, 450, 190, Fade(RAYWHITE, 0.15f));
    DrawText("Shimera - Raylib shape showcase", 18, 16, 18, RAYWHITE);
    DrawText(TextFormat("Effect: %s", effectName), 18, 44, 16, Color{120, 230, 255, 255});
    DrawText("[0] none   [1] bloom  [2] chroma   [3] vignette", 18, 70, 14, GRAY);
    DrawText("[4] pixel  [5] blur   [6] distort  [7] contrast", 18, 90, 14, GRAY);
    DrawText("[8] tint   [9] atmospheric scattering", 18, 110, 14, GRAY);
    DrawText("[C] combo (distortion + chroma)", 18, 130, 14, Color{120, 230, 255, 255});
    DrawText("drag mouse = orbit   wheel = zoom", 18, 158, 14, GRAY);
}

}

int main()
{
    InitWindow(kWidth, kHeight, "Shimera - Raylib shape showcase");

    if (glewInit() != GLEW_OK) {
        std::cerr << "[GLEW] initialization failed!" << '\n';
        return -1;
    }
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';

    const Vector3 target{0.0f, 2.0f, 0.0f};
    float yaw = -0.7f;
    float pitch = 0.4f;
    float dist = 20.0f;

    Camera camera = {0};
    camera.target = target;
    camera.up = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    shimera::IBackend *backend = shimera::BackendFactory::create();
    if (!backend) {
        std::cerr << "Failed to create backend!" << '\n';
        CloseWindow();
        return -1;
    }

    shimera::IFrameBuffer *sceneFb = backend->createFrameBuffer(kWidth, kHeight, true);
    shimera::IFrameBuffer *chainFb = backend->createFrameBuffer(kWidth, kHeight);

    const auto resolution = shimera::Vec2(static_cast<float>(kWidth), static_cast<float>(kHeight));

    shimera::HDRBloomEffect bloom(backend);
    bloom.withThreshold(0.6f)
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
            .withPixelSizeY(6.0f)
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

    shimera::IPostProcessor *passthrough = backend->createPostProcessor(
        "../../../../res/shader/postprocessing/postprocess.vert",
        "../../../../res/shader/postprocessing/normal.frag");

    int mode = 0;
    constexpr std::array<const char *, 11> kModeNames = {
        "None (raw scene)", "HDR Bloom", "Chromatic Aberration", "Vignette",
        "Pixelisation", "Gaussian Blur", "Distortion", "Contrast", "Color Tint",
        "Atmospheric Scattering", "Combo: Distortion + Chroma"};

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        const auto t = static_cast<float>(GetTime());

        // number keys 0-9 select effects 0-9; C selects the chained combo
        for (int i = 0; i <= 9; ++i) {
            if (IsKeyPressed(KEY_ZERO + i)) mode = i;
        }
        if (IsKeyPressed(KEY_C)) mode = 10;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            const Vector2 d = GetMouseDelta();
            yaw -= d.x * 0.005f;
            pitch = std::clamp(pitch - d.y * 0.005f, -1.4f, 1.4f);
        }
        dist = std::clamp(dist - GetMouseWheelMove() * 1.5f, 8.0f, 50.0f);

        camera.position = Vector3{
            target.x + dist * std::cos(pitch) * std::sin(yaw),
            target.y + dist * std::sin(pitch),
            target.z + dist * std::cos(pitch) * std::cos(yaw)};

        sceneFb->bind();
        sceneFb->clear(shimera::Color{0.03f, 0.04f, 0.06f, 1.0f});
        BeginMode3D(camera);
            drawScene(t);
        EndMode3D();
        sceneFb->unbind();


        BeginDrawing();
            ClearBackground(BLACK);
            switch (mode) {
                case 1:  bloom.render(sceneFb->getTexture());                  break;
                case 2:  chroma.render(sceneFb->getTexture());                 break;
                case 3:  vignette.render(sceneFb->getTexture());               break;
                case 4:  pixelise.render(sceneFb->getTexture());               break;
                case 5:  blur.render(sceneFb->getTexture());                   break;
                case 6:  distortion.withTime(t).render(sceneFb->getTexture()); break;
                case 7:  contrast.render(sceneFb->getTexture());               break;
                case 8:  colortint.render(sceneFb->getTexture());              break;
                case 9: {
                    const float aspect = static_cast<float>(kWidth) / static_cast<float>(kHeight);
                    atmosphere.withCamera(toVec3(camera.position), toVec3(target),
                                          camera.fovy, aspect, 0.01f, 1000.0f);
                    atmosphere.withPlanet(toVec3(ringSphereCenter()), kSphereRadius,
                                          kSphereRadius * kAtmosphereScale);
                    atmosphere.setDepthTexture(sceneFb->getDepthTexture());
                    atmosphere.render(sceneFb->getTexture());
                    break;
                }
                case 10:
                    distortion.withTime(t).render(sceneFb->getTexture(), *chainFb);
                    chroma.render(chainFb->getTexture());
                    break;
                default: passthrough->render(sceneFb->getTexture());           break;
            }
            drawHud(kModeNames[static_cast<std::size_t>(mode)]);
        EndDrawing();
    }

    delete passthrough;
    delete chainFb;
    delete sceneFb;
    delete backend;
    CloseWindow();
    return 0;
}
