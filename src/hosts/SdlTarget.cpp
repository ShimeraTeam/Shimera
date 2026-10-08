#include "SdlTarget.hpp"

#include <GL/glew.h>

#include <stdexcept>
#include <string>
#include <string_view>

#include "GL/GLError.hpp"
#include "GL/GLFramebuffer.hpp"

using shimera::SdlTarget;
using shimera::GLTexture;

SdlTarget::SdlTarget(SDL_Renderer& renderer, const int width, const int height)
    : m_renderer(&renderer) {
    // Check if the driver is GL, else throw an error.
    const char* driver = SDL_GetRendererName(m_renderer);
    if (driver == nullptr || std::string_view(driver) != "opengl") {
        throw std::runtime_error(
            "shimera: SdlTarget needs an SDL_Renderer created with the \"opengl\" driver, "
            "got \"" + std::string(driver ? driver : "<unknown>") + "\". Direct3D, Vulkan, "
            "GPU and software renderers keep their textures where a GL core cannot sample "
            "them. Create the renderer with SDL_CreateRenderer(window, \"opengl\").");
    }

    SdlTarget::resize(width, height);
}

SdlTarget::~SdlTarget() {
    // No GLC in a destructor, a dying context makes every call fail, and this must not throw.
    if (m_sdlFbo != 0)
        glDeleteFramebuffers(1, &m_sdlFbo);
    if (m_target != nullptr)
        SDL_DestroyTexture(m_target);
}

GLTexture& SdlTarget::getColor() {
    return m_scene->color();
}

void SdlTarget::begin() {
    /* SDL binds the texture's framebuffer immediately (render-target changes are not
     * queued), so mid-frame raw GL, a material, render correctly in the scene like it should. */
    if (!SDL_SetRenderTarget(m_renderer, m_target))
        throw std::runtime_error(std::string("shimera: SdlTarget::begin() could not set the "
            "render target: ") + SDL_GetError());
}

void SdlTarget::end() {
    /* Back to the backbuffer. This flushes every queued draw command into the texture
     * first, SDL's equivalent of raylib's "rlDrawRenderBatchActive()", and after it the
     * window's framebuffer is bound again, which is exactly where "fx.render()" should
     * composite. */
    if (!SDL_SetRenderTarget(m_renderer, nullptr))
        throw std::runtime_error(std::string("shimera: SdlTarget::end() could not restore the "
            "render target: ") + SDL_GetError());

    /* Unflip. SDL draw the texture at the top-left of the screen, one inverted blit gives the pipeline
     * its GL normal orientation every other host produces. Bindings are restored
     * so SDL's own state cache stays like they were before. */
    const uint32_t previous = currentDrawFramebuffer();
    int previousRead = 0;
    GLC(glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &previousRead));

    GLC(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_sdlFbo));
    GLC(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_scene->handle()));
    GLC(glBlitFramebuffer(0, 0, m_width, m_height,
                          0, m_height, m_width, 0,
                          GL_COLOR_BUFFER_BIT, GL_NEAREST));

    GLC(glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<uint32_t>(previousRead)));
    bindFramebuffer(previous);
}

void SdlTarget::resize(const int width, const int height) {
    m_width = width;
    m_height = height;

    if (m_target != nullptr)
        SDL_DestroyTexture(m_target);

    m_target = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA32,
                                 SDL_TEXTUREACCESS_TARGET, width, height);
    if (m_target == nullptr)
        throw std::runtime_error(std::string("shimera: SdlTarget could not create a ")
            + std::to_string(width) + "x" + std::to_string(height)
            + " render target texture: " + SDL_GetError());

    /* SDL tells us which GL texture backs the SDL_Texture,
     * and from here on it is a GLuint like any other host. */
    const SDL_PropertiesID props = SDL_GetTextureProperties(m_target);
    const auto handle = static_cast<uint32_t>(
        SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_OPENGL_TEXTURE_NUMBER, 0));
    if (handle == 0)
        throw std::runtime_error("shimera: the SDL renderer did not expose a GL texture "
            "behind its render target (driver \""
            + std::string(SDL_GetRendererName(m_renderer)) + "\")");

    m_sdlColor.emplace(handle, width, height);

    // Wrap SDL's texture in a framebuffer of ours so end() can use it as a blit source.
    if (m_sdlFbo == 0)
        GLC(glGenFramebuffers(1, &m_sdlFbo));

    const uint32_t previous = currentDrawFramebuffer();
    GLC(glBindFramebuffer(GL_FRAMEBUFFER, m_sdlFbo));
    GLC(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, handle, 0));
    const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    bindFramebuffer(previous);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("shimera: could not wrap SDL's render target texture in a "
            "framebuffer (status " + std::to_string(status) + ")");

    if (m_scene == nullptr)
        m_scene = std::make_unique<GLFramebuffer>(width, height);
    else
        m_scene->resize(width, height);
}
