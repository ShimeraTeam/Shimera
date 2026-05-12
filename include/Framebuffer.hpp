#ifndef OPENGL_LEARNING_FRAMEBUFFER_H
#define OPENGL_LEARNING_FRAMEBUFFER_H

#include <shimera_api.h>

namespace shimera {

class SHIMERA_API Framebuffer {
public:
    Framebuffer(int w, int h);
    ~Framebuffer();
    void bind() const;
    void unbind() const;
    unsigned int getTexture() const;

private:
    unsigned int m_fbo, m_texture, m_rbo;
    int m_width, m_height;
};

}


#endif //OPENGL_LEARNING_FRAMEBUFFER_H