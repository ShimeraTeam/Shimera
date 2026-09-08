#ifndef SHIMERA_DEMO_CAMERACONTROLLER_HPP
#define SHIMERA_DEMO_CAMERACONTROLLER_HPP

#include "raylib.h"

namespace demo {

// The demo camera: auto-orbital by default, free flight with [R].
class CameraController {
    public:
        CameraController();

        // `blockZoom` keeps the wheel from zooming, e.g. while it is scrolling
        // the shader menu instead.
        void update(bool blockZoom);

        [[nodiscard]] const Camera &camera() const { return m_camera; }
        [[nodiscard]] bool isFreeCam() const { return m_freeCam; }

    private:
        Camera m_camera{};
        bool m_freeCam = false;
        float m_yaw = 0.0f;
        float m_pitch = 0.0f;

        void toggleMode();
        void updateFreeCam();
        void updateOrbital(bool blockZoom);
};

}

#endif //SHIMERA_DEMO_CAMERACONTROLLER_HPP
