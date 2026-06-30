#include <iostream>
#include <vector>
#include <cmath>

#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shimera.h>
#include "backend/BackendFactory.hpp"
#include "converts/GlmConvert.hpp"
#include "scene/Camera.hpp"
#include "effects/materials/FresnelEffect.hpp"

using namespace shimera;

namespace {

constexpr int kWidth = 800;
constexpr int kHeight = 600;

GLFWwindow* initWindow(int width, int height) {
    if (!glfwInit())
        return nullptr;

    GLFWwindow* window = glfwCreateWindow(width, height, "Shimera - Fresnel (3D)", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW ERROR\n";
        return nullptr;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
    return window;
}

// Generates a unit UV-sphere centered at the origin.
// For a unit sphere, the normal at a vertex equals its (normalized) position.
void makeSphere(int stacks, int slices,
                std::vector<float>& positions,
                std::vector<float>& normals,
                std::vector<unsigned int>& indices) {
    for (int i = 0; i <= stacks; ++i) {
        const float v = static_cast<float>(i) / static_cast<float>(stacks);
        const float phi = v * glm::pi<float>();          // 0 .. PI
        for (int j = 0; j <= slices; ++j) {
            const float u = static_cast<float>(j) / static_cast<float>(slices);
            const float theta = u * glm::two_pi<float>(); // 0 .. 2PI

            const float x = std::sin(phi) * std::cos(theta);
            const float y = std::cos(phi);
            const float z = std::sin(phi) * std::sin(theta);

            positions.insert(positions.end(), {x, y, z});
            normals.insert(normals.end(), {x, y, z}); // unit sphere -> position == normal
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

} // namespace

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

    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<unsigned int> indices;
    makeSphere(64, 64, positions, normals, indices);

    IMesh* sphere = backend->createMesh(positions, normals, indices);

    FresnelEffect fresnel(backend);
    fresnel.withColor(Vec3(0.3f, 0.6f, 1.0f))
           .withPower(3.0f)
           .withReflectance(0.04f)
           .withIntensity(1.5f);

    // Static camera looking at the origin.
    Camera camera;
    camera.position = Vec3(0.0f, 0.0f, 3.5f);
    camera.view = toShimMat4(glm::lookAt(
        glm::vec3(camera.position.x, camera.position.y, camera.position.z),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)));
    camera.projection = toShimMat4(glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(kWidth) / static_cast<float>(kHeight),
        0.1f, 100.0f));

    glEnable(GL_DEPTH_TEST);

    float angle = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.02f, 0.02f, 0.04f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Spin the sphere so the rim/grazing-angle highlight is easy to see.
        angle += 0.01f;
        const glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        fresnel.setModel(toShimMat4(model));

        fresnel.render(*sphere, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete sphere;
    delete backend;
    glfwTerminate();
    return 0;
}
