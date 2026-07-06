#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <shimera.h>
#include "backend/BackendFactory.hpp"
#include "scene/Camera.hpp"
#include "scene/CameraFactory.hpp"
#include "effects/materials/FresnelEffect.hpp"

using namespace shimera;

namespace {

constexpr int kWidth = 1280;
constexpr int kHeight = 720;

struct FlyCamera {
    glm::vec3 position{0.0f, 0.0f, 6.0f};
    float yaw = -90.0f;
    float pitch = 0.0f;
    float speed = 6.0f;
    float sensitivity = 0.1f;

    [[nodiscard]] glm::vec3 front() const {
        return glm::normalize(glm::vec3(
            std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch)),
            std::sin(glm::radians(pitch)),
            std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch))));
    }
    [[nodiscard]] glm::vec3 right() const {
        return glm::normalize(glm::cross(front(), glm::vec3(0.0f, 1.0f, 0.0f)));
    }
};

GLFWwindow* initWindow(int width, int height) {
    if (!glfwInit())
        return nullptr;

    GLFWwindow* window = glfwCreateWindow(width, height, "Shimera - Fresnel (OpenGL, fly camera)", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW ERROR\n";
        return nullptr;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
    return window;
}

void makeSphere(int stacks, int slices,
                std::vector<float>& positions,
                std::vector<float>& normals,
                std::vector<unsigned int>& indices) {
    for (int i = 0; i <= stacks; ++i) {
        const float phi = static_cast<float>(i) / static_cast<float>(stacks) * glm::pi<float>();
        for (int j = 0; j <= slices; ++j) {
            const float theta = static_cast<float>(j) / static_cast<float>(slices) * glm::two_pi<float>();
            const float x = std::sin(phi) * std::cos(theta);
            const float y = std::cos(phi);
            const float z = std::sin(phi) * std::sin(theta);
            positions.insert(positions.end(), {x, y, z});
            normals.insert(normals.end(), {x, y, z});
        }
    }
    const int ring = slices + 1;
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            const unsigned int a = i * ring + j;
            const unsigned int b = a + ring;
            indices.insert(indices.end(), {a, b, a + 1, a + 1, b, b + 1});
        }
    }
}

void makeCube(float half,
              std::vector<float>& positions,
              std::vector<float>& normals,
              std::vector<unsigned int>& indices) {
    const auto addQuad = [&](glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec3 n) {
        const unsigned int base = static_cast<unsigned int>(positions.size() / 3);
        for (const glm::vec3& v : {a, b, c, d}) {
            positions.insert(positions.end(), {v.x, v.y, v.z});
            normals.insert(normals.end(), {n.x, n.y, n.z});
        }
        indices.insert(indices.end(), {base, base + 1, base + 2, base, base + 2, base + 3});
    };
    const float h = half;
    addQuad({-h,-h, h}, { h,-h, h}, { h, h, h}, {-h, h, h}, { 0, 0, 1}); // +Z
    addQuad({ h,-h,-h}, {-h,-h,-h}, {-h, h,-h}, { h, h,-h}, { 0, 0,-1}); // -Z
    addQuad({-h,-h,-h}, {-h,-h, h}, {-h, h, h}, {-h, h,-h}, {-1, 0, 0}); // -X
    addQuad({ h,-h, h}, { h,-h,-h}, { h, h,-h}, { h, h, h}, { 1, 0, 0}); // +X
    addQuad({-h, h, h}, { h, h, h}, { h, h,-h}, {-h, h,-h}, { 0, 1, 0}); // +Y
    addQuad({-h,-h,-h}, { h,-h,-h}, { h,-h, h}, {-h,-h, h}, { 0,-1, 0}); // -Y
}

void processKeyboard(GLFWwindow* window, FlyCamera& cam, float dt) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    const float step = cam.speed * dt;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cam.position += cam.front() * step; // Z: forward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cam.position -= cam.front() * step; // S: back
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cam.position -= cam.right() * step; // Q: left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cam.position += cam.right() * step; // D: right
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)        cam.position.y += step;  // up
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)   cam.position.y -= step;  // down
}

}

int main() {
    GLFWwindow* window = initWindow(kWidth, kHeight);
    if (!window)
        return -1;

    IBackend* backend = BackendFactory::create();
    if (!backend) {
        std::cerr << "Failed to create backend!\n";
        glfwTerminate();
        return -1;
    }

    std::vector<float> spPos, spNrm; std::vector<unsigned int> spIdx;
    makeSphere(64, 64, spPos, spNrm, spIdx);
    IMesh* sphere = backend->createMesh(spPos, spNrm, spIdx);

    std::vector<float> cbPos, cbNrm; std::vector<unsigned int> cbIdx;
    makeCube(1.0f, cbPos, cbNrm, cbIdx);
    IMesh* cube = backend->createMesh(cbPos, cbNrm, cbIdx);

    // --- One Fresnel material, reused for every object ---
    FresnelEffect fresnel(backend);
    fresnel.withColor(Vec3(0.3f, 0.6f, 1.0f))
           .withPower(3.0f)
           .withReflectance(0.04f)
           .withIntensity(1.5f);

    glEnable(GL_DEPTH_TEST);

    std::cout << "Controls: ZQSD move, mouse look, Space/Shift up-down, Esc quit\n";

    float last = static_cast<float>(glfwGetTime());
    FlyCamera cam;
    bool firstMouse = true;
    double lastX = 0.0, lastY = 0.0;

    while (!glfwWindowShouldClose(window)) {
        const float now = static_cast<float>(glfwGetTime());
        const float dt = now - last;
        last = now;

        // --- Mouse look ---
        double mx = 0.0, my = 0.0;
        glfwGetCursorPos(window, &mx, &my);
        if (firstMouse) { lastX = mx; lastY = my; firstMouse = false; }
        cam.yaw   += static_cast<float>(mx - lastX) * cam.sensitivity;
        cam.pitch += static_cast<float>(lastY - my) * cam.sensitivity; // y is inverted
        cam.pitch = std::clamp(cam.pitch, -89.0f, 89.0f);
        lastX = mx; lastY = my;

        processKeyboard(window, cam, dt);

        // Keep the viewport/aspect correct on resize.
        int fbw = 0, fbh = 0;
        glfwGetFramebufferSize(window, &fbw, &fbh);
        glViewport(0, 0, fbw, fbh);
        const float aspect = fbh > 0 ? static_cast<float>(fbw) / static_cast<float>(fbh) : 1.0f;

        // --- Build the Shimera camera from the fly-camera (glm stays internal to CameraFactory) ---
        const glm::vec3 f = cam.front();
        const Camera camera = CameraFactory::perspective(
            Vec3(cam.position.x, cam.position.y, cam.position.z),
            Vec3(cam.position.x + f.x, cam.position.y + f.y, cam.position.z + f.z),
            Vec3(0.0f, 1.0f, 0.0f),
            45.0f, aspect, 0.1f, 100.0f);

        glClearColor(0.02f, 0.02f, 0.04f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Same material, three objects at different positions.
        fresnel.setTransform(Vec3(0.0f, 3.0f, 6.0f));
        fresnel.render(*sphere, camera);

        fresnel.setTransform(Vec3(3.0f, 0.0f, 0.0f));
        fresnel.render(*cube, camera);

        fresnel.setTransform(Vec3(-3.0f, 0.0f, 0.0f));
        fresnel.render(*sphere, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete sphere;
    delete cube;
    delete backend;
    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
}
