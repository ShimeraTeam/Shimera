#ifndef SHIMERA_FRAMEBUFFER_HPP
#define SHIMERA_FRAMEBUFFER_HPP

#include <shimera_api.h>

namespace shimera {

class SHIMERA_API Framebuffer {
public:
    Framebuffer(int w, int h);
    ~Framebuffer();
    void bind() const;
    void unbind() const;
    [[nodiscard]] unsigned int getTexture() const;

private:
    unsigned int m_fbo, m_texture, m_rbo;
    int m_width, m_height;
};

}


#endif //SHIMERA_FRAMEBUFFER_HPP