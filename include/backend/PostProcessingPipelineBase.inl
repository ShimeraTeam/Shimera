/**
* DEPRECATED: This class is deprecated. It was the first implementation of post-processing Main/Base abstraction.
* Being only Single-Pass, it is now replaced by `BackendFactory` that is Multi-Pass capable.
* It is kept for proof of work purposes only.
*/

#ifndef SHIMERA_POSTPROCESSINGPIPELINE_INL
#define SHIMERA_POSTPROCESSINGPIPELINE_INL
#include <map>
#include <string>

#include "uniform/Uniform.inl"

template<typename C>
class PostProcessingPipelineBase {
    public:
        C &setUniform(const std::string &name, const UniformValue &value) {
            uniforms[name] = value;
            static_cast<C*>(this)->applyUniform(name, value);
            return *static_cast<C*>(this);
        }
        void resize(int w, int h) {
            width = w;
            height = h;
            static_cast<C*>(this)->onResize(w, h);
        }

    protected:
        std::map<std::string, UniformValue> uniforms;
        int width = 0;
        int height = 0;
};

#endif //SHIMERA_POSTPROCESSINGPIPELINE_INL