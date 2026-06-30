#include "backend/raylib/converts/RaylibCamera.hpp"

#include "scene/CameraFactory.hpp"

shimera::Camera shimera::RaylibCamera::toShimera(const Camera3D& cam, float nearPlane, float farPlane, float aspect) {
    if (aspect <= 0.0f) {
        aspect = static_cast<float>(GetScreenWidth()) /
                 static_cast<float>(GetScreenHeight());
    }

    return CameraFactory::perspective(
        {cam.position.x, cam.position.y, cam.position.z},
        {cam.target.x, cam.target.y, cam.target.z},
        {cam.up.x, cam.up.y, cam.up.z},
        cam.fovy, aspect, nearPlane, farPlane);
}
