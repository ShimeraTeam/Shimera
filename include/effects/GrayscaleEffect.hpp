#ifndef SHIMERA_GRAYSCALEEFFECT_HPP
#define SHIMERA_GRAYSCALEEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

namespace shimera {

class SHIMERA_API GrayscaleEffect final : public ShaderEffect<GrayscaleEffect> {
    public:
        explicit GrayscaleEffect(IBackend *backend);

        void updateUniforms() override;

        std::string getName() const override;
};

}

#endif //SHIMERA_GRAYSCALEEFFECT_HPP
