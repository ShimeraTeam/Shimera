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

#ifndef OPENGL_LEARNING_FRAMEBUFFER_H
#define OPENGL_LEARNING_FRAMEBUFFER_H

#include <shimera_api.h>

namespace shimera {

class SHIMERA_API Framebuffer {
public:
    Framebuffer(int w, int h);
    ~Framebuffer();
    void bind() const;
    void unbind() const;
    [[nodiscard]] unsigned int getTexture() const;

private:
    unsigned int m_fbo, m_texture, m_rbo;
    int m_width, m_height;
};

}


#endif //OPENGL_LEARNING_FRAMEBUFFER_H