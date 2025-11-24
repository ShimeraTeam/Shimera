//
// Created by leoma on 24/11/2025.
//

#ifndef OPENGL_LEARNING_POSTPROCESSINGQUAD_H
#define OPENGL_LEARNING_POSTPROCESSINGQUAD_H
#include <string>


class PostProcessingQuad {
    public:
        PostProcessingQuad(const std::string& vertPath, const std::string& fragPath);
        ~PostProcessingQuad();
        void render(unsigned int texture) const;
        unsigned int getShader() const;

    private:
        unsigned int vao, vbo, ebo;
        unsigned int shader;
};


#endif //OPENGL_LEARNING_POSTPROCESSINGQUAD_H