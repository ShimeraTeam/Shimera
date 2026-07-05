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
