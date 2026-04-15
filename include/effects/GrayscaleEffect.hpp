#ifndef SHIMERA_GRAYSCALEEFFECT_HPP
#define SHIMERA_GRAYSCALEEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

class SHIMERA_API GrayscaleEffect final : public ShaderEffect<GrayscaleEffect> {
    public:
        explicit GrayscaleEffect(IBackend *backend);

        void updateUniforms() override;
};

#endif //SHIMERA_GRAYSCALEEFFECT_HPP
