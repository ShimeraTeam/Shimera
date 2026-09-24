#include "OpenGLMain.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <numbers>

namespace {

constexpr int kWindowWidth = 960;
constexpr int kWindowHeight = 540;

constexpr const char* kBackgroundPath = "res/imgs/pattern_checkerboard_gray.png";

constexpr float kBackgroundTileSize = 24.0f;
constexpr float kFloorHalfExtent = 400.0f;

const glm::vec3 kClearColor{18.0f / 255.0f, 20.0f / 255.0f, 28.0f / 255.0f};

const glm::vec3 kBoxSize{2.8f, 2.1f, 2.1f};
const glm::vec3 kBoxPosition{-1.95f, 1.05f, 0.0f};
const glm::vec3 kBoxColor{214.0f / 255.0f, 79.0f / 255.0f, 56.0f / 255.0f};
const glm::vec3 kOutlineColor{1.0f, 236.0f / 255.0f, 214.0f / 255.0f};

constexpr float kSphereRadius = 1.15f;
const glm::vec3 kSpherePosition{1.95f, 1.15f, 0.0f};
const glm::vec3 kSphereColor{0.16f, 0.71f, 0.95f};
constexpr int kSphereSlices = 96;
constexpr int kSphereStacks = 48;

const glm::vec3 kLightDirection = glm::normalize(glm::vec3{-0.4f, -1.0f, -0.3f});

const glm::vec3 kCameraTarget{0.0f, 1.0f, 0.0f};
constexpr float kCameraHeight = 5.0f;
constexpr float kCameraDistance = 9.0f;
constexpr float kCameraOrbitSpeed = 0.3f; // radians per second
constexpr float kCameraFov = 45.0f;

constexpr float kDistortionSpeed = 1000.0f / 300.0f;

// language=GLSL
constexpr const char* kVertexShader = R"(#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_viewProjection;

out vec3 v_normal;
out vec2 v_uv;

void main() {
    v_normal = mat3(u_model) * a_normal;
    v_uv = a_uv;
    gl_Position = u_viewProjection * u_model * vec4(a_position, 1.0);
}
)";

// language=GLSL
constexpr const char* kFragmentShader = R"(#version 330 core
in vec3 v_normal;
in vec2 v_uv;

uniform int u_shading; // 0 lit, 1 unlit, 2 textured
uniform vec3 u_color;
uniform vec3 u_lightDirection;
uniform sampler2D u_texture;

out vec4 o_color;

void main() {
    if (u_shading == 2) {
        o_color = vec4(texture(u_texture, v_uv).rgb, 1.0);
    } else if (u_shading == 1) {
        o_color = vec4(u_color, 1.0);
    } else {
        float diffuse = max(dot(normalize(v_normal), -u_lightDirection), 0.0);
        o_color = vec4(u_color * (0.35 + 0.65 * diffuse), 1.0);
    }
}
)";

void onGlfwError(const int code, const char* description) {
    std::cerr << "GLFW error " << code << ": " << description << '\n';
}

GLuint compileShader(const GLenum stage, const char* source) {
    const GLuint shader = glCreateShader(stage);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_TRUE)
        return shader;

    std::array<char, 1024> log{};
    glGetShaderInfoLog(shader, static_cast<GLsizei>(log.size()), nullptr, log.data());
    std::cerr << (stage == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
              << " shader failed to compile:\n" << log.data() << '\n';
    glDeleteShader(shader);
    return 0;
}

glm::vec3 cameraEye(const float time) {
    const float angle = kCameraOrbitSpeed * time + std::numbers::pi_v<float> / 4.0f;
    return {std::cos(angle) * kCameraDistance, kCameraHeight, std::sin(angle) * kCameraDistance};
}

shimera::Mat4 toShimera(const glm::mat4& matrix) {
    shimera::Mat4 result{};
    std::copy_n(glm::value_ptr(matrix), 16, result.m);
    return result;
}

shimera::Vec3<float> toShimera(const glm::vec3& vector) {
    return {vector.x, vector.y, vector.z};
}

} // namespace

OpenGLMain::~OpenGLMain() {
    m_fresnel.reset();
    m_sphere.reset();
    m_fx.reset();
    m_scene.reset();
    m_context.reset();

    // Every handle is still 0 if init() stopped before GL was loaded, so nothing is called.
    destroyMesh(m_boxEdges);
    destroyMesh(m_box);
    destroyMesh(m_floor);
    if (m_background != 0)
        glDeleteTextures(1, &m_background);
    if (m_program != 0)
        glDeleteProgram(m_program);

    if (m_window != nullptr)
        glfwDestroyWindow(m_window);

    glfwTerminate();
}

bool OpenGLMain::init(const char* title, const int width, const int height) {
    glfwSetErrorCallback(onGlfwError);
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (m_window == nullptr)
        return false;

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (const GLenum error = glewInit(); error != GLEW_OK) {
        std::cerr << "glewInit failed: " << reinterpret_cast<const char*>(glewGetErrorString(error))
                  << '\n';
        return false;
    }

    if (!createShader() || !loadBackground(kBackgroundPath))
        return false;

    m_floor = createFloor();
    m_box = createBox(kBoxSize);
    m_boxEdges = createBoxEdges(kBoxSize);

    glfwGetFramebufferSize(m_window, &m_width, &m_height);

    m_context.emplace(shimera::Context::create());
    std::cout << shimera::Context::getRendererInfo() << '\n';

    m_scene.emplace(m_width, m_height);

    m_fx.emplace(m_width, m_height);
    m_fx->add<shimera::Vignette>(1.0f, 0.45f, 0.35f)
        .add<shimera::Distortion>(1.0f, 0.1f, 0.1f)
        .add<shimera::HDRBloom>();

    const Geometry sphere = createSphere(kSphereRadius, kSphereSlices, kSphereStacks);
    m_sphere.emplace(sphere.positions, sphere.normals, sphere.indices);

    m_fresnel.emplace(toShimera(kSphereColor));
    m_fresnel->getTransform().position = toShimera(kSpherePosition);

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, const int w, const int h) {
        static_cast<OpenGLMain*>(glfwGetWindowUserPointer(window))->resize(w, h);
    });

    return true;
}

bool OpenGLMain::createShader() {
    const GLuint vertex = compileShader(GL_VERTEX_SHADER, kVertexShader);
    const GLuint fragment = compileShader(GL_FRAGMENT_SHADER, kFragmentShader);
    if (vertex == 0 || fragment == 0) {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return false;
    }

    m_program = glCreateProgram();
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);
    glLinkProgram(m_program);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    GLint linked = GL_FALSE;
    glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        std::array<char, 1024> log{};
        glGetProgramInfoLog(m_program, static_cast<GLsizei>(log.size()), nullptr, log.data());
        std::cerr << "Shader program failed to link:\n" << log.data() << '\n';
        return false;
    }

    m_uniforms.model = glGetUniformLocation(m_program, "u_model");
    m_uniforms.viewProjection = glGetUniformLocation(m_program, "u_viewProjection");
    m_uniforms.color = glGetUniformLocation(m_program, "u_color");
    m_uniforms.shading = glGetUniformLocation(m_program, "u_shading");
    m_uniforms.lightDirection = glGetUniformLocation(m_program, "u_lightDirection");

    glUseProgram(m_program);
    glUniform1i(glGetUniformLocation(m_program, "u_texture"), 0);
    glUseProgram(0);

    return true;
}

bool OpenGLMain::loadBackground(const char* path) {
    int width = 0;
    int height = 0;
    int channels = 0;

    unsigned char* pixels = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
    if (pixels == nullptr) {
        std::cerr << "stbi_load(\"" << path << "\") failed: " << stbi_failure_reason() << '\n';
        return false;
    }

    glGenTextures(1, &m_background);
    glBindTexture(GL_TEXTURE_2D, m_background);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    stbi_image_free(pixels);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if (GLEW_EXT_texture_filter_anisotropic) {
        GLfloat maxAnisotropy = 1.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

OpenGLMain::Mesh OpenGLMain::createMesh(const std::vector<Vertex>& vertices,
                                        const std::vector<GLuint>& indices, const GLenum mode) {
    Mesh mesh;
    mesh.indexCount = static_cast<GLsizei>(indices.size());
    mesh.mode = mode;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(GLuint)),
                 indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<const void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<const void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<const void*>(offsetof(Vertex, uv)));

    // The VAO first
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return mesh;
}

OpenGLMain::Mesh OpenGLMain::createFloor() {
    const float extent = kFloorHalfExtent;
    const float repeats = kFloorHalfExtent / kBackgroundTileSize;
    const glm::vec3 up{0.0f, 1.0f, 0.0f};

    const std::vector<Vertex> vertices{
        {{-extent, 0.0f, extent}, up, {-repeats, -repeats}},
        {{extent, 0.0f, extent}, up, {repeats, -repeats}},
        {{extent, 0.0f, -extent}, up, {repeats, repeats}},
        {{-extent, 0.0f, -extent}, up, {-repeats, repeats}},
    };
    const std::vector<GLuint> indices{0, 1, 2, 0, 2, 3};

    return createMesh(vertices, indices, GL_TRIANGLES);
}

OpenGLMain::Mesh OpenGLMain::createBox(const glm::vec3& size) {
    struct Face {
        glm::vec3 normal;
        glm::vec3 right;
        glm::vec3 up;
    };
    constexpr std::array<Face, 6> faces{{
        {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},
        {{-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{0.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    }};
    constexpr std::array<glm::vec2, 4> corners{{{-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 1.0f}}};
    const glm::vec3 half = size * 0.5f;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    for (const Face& face : faces) {
        const auto base = static_cast<GLuint>(vertices.size());
        for (const glm::vec2& corner : corners) {
            const glm::vec3 position = (face.normal + face.right * corner.x + face.up * corner.y) * half;
            vertices.push_back({position, face.normal, (corner + 1.0f) * 0.5f});
        }
        indices.insert(indices.end(), {base, base + 1, base + 2, base, base + 2, base + 3});
    }

    return createMesh(vertices, indices, GL_TRIANGLES);
}

OpenGLMain::Mesh OpenGLMain::createBoxEdges(const glm::vec3& size) {
    const glm::vec3 half = size * 0.5f;

    std::vector<Vertex> vertices;
    for (int i = 0; i < 8; ++i) {
        const glm::vec3 sign{(i & 1) ? 1.0f : -1.0f, (i & 2) ? 1.0f : -1.0f, (i & 4) ? 1.0f : -1.0f};
        vertices.push_back({half * sign, glm::vec3{0.0f}, glm::vec2{0.0f}});
    }

    std::vector<GLuint> indices;
    for (GLuint corner = 0; corner < 8; ++corner) {
        for (GLuint axis = 1; axis < 8; axis <<= 1) {
            if ((corner & axis) == 0)
                indices.insert(indices.end(), {corner, corner | axis});
        }
    }

    return createMesh(vertices, indices, GL_LINES);
}

OpenGLMain::Geometry OpenGLMain::createSphere(const float radius, const int slices, const int stacks) {
    constexpr float pi = std::numbers::pi_v<float>;
    const auto vertexCount = static_cast<size_t>((slices + 1) * (stacks + 1));

    Geometry geometry;
    geometry.positions.reserve(vertexCount * 3);
    geometry.normals.reserve(vertexCount * 3);
    for (int stack = 0; stack <= stacks; ++stack) {
        const float phi = pi * static_cast<float>(stack) / static_cast<float>(stacks); // 0 at the top pole
        for (int slice = 0; slice <= slices; ++slice) {
            const float theta = 2.0f * pi * static_cast<float>(slice) / static_cast<float>(slices);
            const glm::vec3 normal{std::sin(phi) * std::cos(theta), std::cos(phi),
                                   std::sin(phi) * std::sin(theta)};
            const glm::vec3 position = normal * radius;
            geometry.positions.insert(geometry.positions.end(), {position.x, position.y, position.z});
            geometry.normals.insert(geometry.normals.end(), {normal.x, normal.y, normal.z});
        }
    }

    geometry.indices.reserve(static_cast<size_t>(slices * stacks * 6));
    const auto row = static_cast<unsigned int>(slices + 1);
    for (int stack = 0; stack < stacks; ++stack) {
        for (int slice = 0; slice < slices; ++slice) {
            const unsigned int a = static_cast<unsigned int>(stack) * row + static_cast<unsigned int>(slice);
            const unsigned int b = a + row;
            geometry.indices.insert(geometry.indices.end(), {a, a + 1, b, a + 1, b + 1, b});
        }
    }

    return geometry;
}

void OpenGLMain::destroyMesh(Mesh& mesh) {
    if (mesh.vao == 0)
        return;

    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ebo);
    mesh = {};
}

void OpenGLMain::run() {
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_window, GLFW_TRUE);

        const auto time = static_cast<float>(glfwGetTime());
        m_fx->get<shimera::Distortion>().withTime(time * kDistortionSpeed);

        draw(time);
    }
}

void OpenGLMain::resize(const int width, const int height) {
    if (width <= 0 || height <= 0)
        return;

    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);

    m_scene->resize(width, height);
    m_fx->resize(width, height);
}

void OpenGLMain::draw(const float time) {
    m_scene->begin();

    glClearColor(kClearColor.r, kClearColor.g, kClearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    const float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
    const glm::vec3 eye = cameraEye(time);
    const glm::mat4 view = glm::lookAt(eye, kCameraTarget, glm::vec3{0.0f, 1.0f, 0.0f});
    const glm::mat4 projection = glm::perspective(glm::radians(kCameraFov), aspect, 0.1f, 1000.0f);
    const glm::mat4 viewProjection = projection * view;

    glUseProgram(m_program);
    glUniformMatrix4fv(m_uniforms.viewProjection, 1, GL_FALSE, glm::value_ptr(viewProjection));
    glUniform3fv(m_uniforms.lightDirection, 1, glm::value_ptr(kLightDirection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_background);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);
    drawMesh(m_floor, glm::vec3{0.0f}, glm::vec3{1.0f}, Shading::Textured);
    drawMesh(m_box, kBoxPosition, kBoxColor, Shading::Lit);
    glDisable(GL_POLYGON_OFFSET_FILL);
    drawMesh(m_boxEdges, kBoxPosition, kOutlineColor, Shading::Unlit);

    glBindVertexArray(0);
    glUseProgram(0);

    // Drawn last, as it blends over what is behind it.
    m_fresnel->render(*m_sphere, shimera::Camera::fromMatrices(toShimera(view), toShimera(projection),
                                                                toShimera(eye)));

    // Must be done in OpenGL, else it will display a black screen.
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    m_scene->end();

    m_fx->render(*m_scene);

    glfwSwapBuffers(m_window);
}

void OpenGLMain::drawMesh(const Mesh& mesh, const glm::vec3& position, const glm::vec3& color,
                          const Shading shading) const {
    const glm::mat4 model = glm::translate(glm::mat4{1.0f}, position);
    glUniformMatrix4fv(m_uniforms.model, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(m_uniforms.color, 1, glm::value_ptr(color));
    glUniform1i(m_uniforms.shading, static_cast<GLint>(shading));

    glBindVertexArray(mesh.vao);
    glDrawElements(mesh.mode, mesh.indexCount, GL_UNSIGNED_INT, nullptr);
}

int main() {
    OpenGLMain example;
    if (!example.init("Shimera - OpenGL 3D example", kWindowWidth, kWindowHeight))
        return 1;

    example.run();
    return 0;
}
