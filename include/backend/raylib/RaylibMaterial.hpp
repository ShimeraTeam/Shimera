#ifndef SHIMERA_RAYLIBMATERIAL_HPP
#define SHIMERA_RAYLIBMATERIAL_HPP

#include <raylib.h>
#include <unordered_map>

#include "backend/IMaterial.hpp"

namespace shimera {

class SHIMERA_API RaylibMaterial : public IMaterial {
    public:
        RaylibMaterial(const std::string& vert, const std::string& frag);
        ~RaylibMaterial() override;

        void setUniform(const std::string& name, const UniformValue& value) override;
        [[nodiscard]] Shader shader() const;
    private:
        Shader m_shader{};
        std::pmr::unordered_map<std::string, int> m_locs;

        void bindShimeraConventions();
        int locOf(const std::string& name);
};

}

#endif //SHIMERA_RAYLIBMATERIAL_HPP
