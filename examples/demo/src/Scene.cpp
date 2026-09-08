#include "Scene.hpp"

#include "raylib.h"
#include "rlgl.h"

#include "DemoConfig.hpp"

namespace demo {
namespace {

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

}
