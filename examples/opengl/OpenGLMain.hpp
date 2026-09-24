#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <shimera.hpp>
#include <hosts/GlfwTarget.hpp>

#include <optional>
#include <vector>

/* A standalone OpenGL 3D example, windowed with GLFW. */
class OpenGLMain {
public:
    OpenGLMain() = default;
    ~OpenGLMain();

    OpenGLMain(const OpenGLMain&) = delete;
    OpenGLMain& operator=(const OpenGLMain&) = delete;

    bool init(const char* title, int width, int height);

    void run();

private:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    struct Mesh {
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ebo = 0;
        GLsizei indexCount = 0;
        GLenum mode = GL_TRIANGLES;
    };

    struct Uniforms {
        GLint model = -1;
        GLint viewProjection = -1;
        GLint color = -1;
        GLint shading = -1;
        GLint lightDirection = -1;
    };

    struct Geometry {
        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<unsigned int> indices;
    };

    enum class Shading : GLint { Lit = 0, Unlit = 1, Textured = 2 };

    static Mesh createMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices,
                           GLenum mode);
    static Mesh createFloor();
    static Mesh createBox(const glm::vec3& size);
    static Mesh createBoxEdges(const glm::vec3& size);
    static Geometry createSphere(float radius, int slices, int stacks);
    static void destroyMesh(Mesh& mesh);

    bool createShader();
    bool loadBackground(const char* path);
    void resize(int width, int height);

    void draw(float time);
    void drawMesh(const Mesh& mesh, const glm::vec3& position, const glm::vec3& color,
                  Shading shading) const;

    GLFWwindow* m_window = nullptr;
    int m_width = 0;
    int m_height = 0;

    GLuint m_program = 0;
    Uniforms m_uniforms;
    GLuint m_background = 0;

    Mesh m_floor;
    Mesh m_box;
    Mesh m_boxEdges;

    std::optional<shimera::Context> m_context;
    std::optional<shimera::GlfwTarget> m_scene;
    std::optional<shimera::EffectPipeline> m_fx;

    std::optional<shimera::GLMesh> m_sphere;
    std::optional<shimera::Fresnel> m_fresnel;
};
