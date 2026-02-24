#ifndef OPENGL_LEARNING_FRAMEBUFFER_H
#define OPENGL_LEARNING_FRAMEBUFFER_H

#include <shimera_api.h>

class SHIMERA_API Framebuffer
{
    public:
        Framebuffer(int w, int h);
        ~Framebuffer();
        void bind() const;
        void unbind() const;
        unsigned int getTexture() const;

    private:
        unsigned int fbo, texture, rbo;
        int width, height;
};


#endif //OPENGL_LEARNING_FRAMEBUFFER_H