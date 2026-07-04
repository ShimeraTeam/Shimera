// SPDX-License-Identifier: GPL-3.0-only
//
// Shimera: a simple way to add visual effects without using any GPU knowledge
// Copyright (C) 2025-2026 The Shimera Authors
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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

namespace shimera {

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

}

#endif //SHIMERA_POSTPROCESSINGPIPELINE_INL