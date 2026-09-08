#include "CameraController.hpp"

#include <algorithm>
#include <cmath>

#include "DemoConfig.hpp"
#include "MathUtils.hpp"

namespace demo {

CameraController::CameraController()
{
    m_camera.position = Vector3{12.0f, 8.0f, 16.0f};
    m_camera.target = kBoxCenter;
    m_camera.up = Vector3{0.0f, 1.0f, 0.0f};
    m_camera.fovy = 45.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

void CameraController::update(bool blockZoom)
{
    if (IsKeyPressed(KEY_R))
        toggleMode();

    if (m_freeCam)
        updateFreeCam();
    else
        updateOrbital(blockZoom);
}

void CameraController::toggleMode()
{
    m_freeCam = !m_freeCam;
    if (m_freeCam) {
        DisableCursor();
        // Start the free flight looking exactly where the orbital camera was.
        const Vector3 d = add(m_camera.target, scale(m_camera.position, -1.0f));
        m_yaw = std::atan2(d.x, d.z);
        m_pitch = std::asin(d.y / length(d));
    } else {
        EnableCursor();
        m_camera.target = kBoxCenter;
    }
}

void CameraController::updateFreeCam()
{
    const auto [x, y] = GetMouseDelta();
    m_yaw -= x * 0.003f;
    m_pitch = std::clamp(m_pitch - y * 0.003f, -1.5f, 1.5f);

    const Vector3 forward{std::cos(m_pitch) * std::sin(m_yaw), std::sin(m_pitch),
                          std::cos(m_pitch) * std::cos(m_yaw)};
    const float flatLen = std::max(std::sqrt(forward.x * forward.x + forward.z * forward.z), 0.001f);
    const Vector3 right{-forward.z / flatLen, 0.0f, forward.x / flatLen};

    Vector3 move{0, 0, 0};
    if (IsKeyDown(KEY_W)) move = add(move, forward);
    if (IsKeyDown(KEY_S)) move = add(move, scale(forward, -1.0f));
    if (IsKeyDown(KEY_A)) move = add(move, scale(right, -1.0f));
    if (IsKeyDown(KEY_D)) move = add(move, right);
    if (IsKeyDown(KEY_SPACE)) move.y += 1.0f;
    if (IsKeyDown(KEY_LEFT_SHIFT)) move.y -= 1.0f;

    m_camera.position = add(m_camera.position, scale(move, 8.0f * GetFrameTime()));
    m_camera.target = add(m_camera.position, forward);
}

void CameraController::updateOrbital(bool blockZoom)
{
    // Orbital also zooms with the wheel: restore the distance afterwards when
    // the wheel is meant for something else.
    const Vector3 before = add(m_camera.position, scale(m_camera.target, -1.0f));
    const float distance = length(before);

    UpdateCamera(&m_camera, CAMERA_ORBITAL);

    if (!blockZoom)
        return;
    const Vector3 after = add(m_camera.position, scale(m_camera.target, -1.0f));
    m_camera.position = add(m_camera.target, scale(after, distance / std::max(length(after), 0.001f)));
}

}
