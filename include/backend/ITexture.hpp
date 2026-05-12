#ifndef SHIMERA_ITEXTURE_HPP
#define SHIMERA_ITEXTURE_HPP

#include <shimera_api.h>

namespace shimera {

class SHIMERA_API ITexture {
    public:
        virtual ~ITexture() = default;

        virtual void bind(int slot = 0) = 0;
        virtual void unbind() = 0;
        [[nodiscard]] virtual unsigned int getNativeHandle() const = 0;
        [[nodiscard]] virtual int getWidth() const = 0;
        [[nodiscard]] virtual int getHeight() const = 0;
};

}

#endif //SHIMERA_ITEXTURE_HPP