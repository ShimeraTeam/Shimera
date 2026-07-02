#ifndef SHIMERA_POSTPROCESSINGQUAD_HPP
#define SHIMERA_POSTPROCESSINGQUAD_HPP

#include <shimera_api.h>
#include <string>

namespace shimera {

class SHIMERA_API PostProcessingQuad {
    public:
        PostProcessingQuad(const std::string& vertPath, const std::string& fragPath);
        ~PostProcessingQuad();
        void render(unsigned int texture) const;
        [[nodiscard]] unsigned int getShader() const;
        void bindShader() const;

    private:
        unsigned int m_vao, m_vbo, m_ebo;
        unsigned int m_shader;
};

}


#endif //SHIMERA_POSTPROCESSINGQUAD_HPP
