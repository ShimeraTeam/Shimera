// SPDX-License-Identifier: GPL-3.0-only
//
// Shimera: a simple way to add visual effects without any GPU knowledge
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

#include "UniformBlock.hpp"

#include <GL/glew.h>
#include "GL/GLError.hpp"

#include <cstring>
#include <string>
#include <utility>
#include <variant>

#include "Vec2.inl"
#include "Vec3.inl"
#include "Vec4.inl"
#include "Mat4.hpp"

using shimera::UniformBlock;

std::string UniformBlock::normalizeName(std::string name) {
    /* GL reports "BlockName.member" when the block has an instance name, which Slang's
     * output always does. Effects know the bare name, so drop the first segment. */
    if (const auto dot = name.find('.'); dot != std::string::npos)
        name.erase(0, dot + 1);

    /* SPIRV-Cross wraps every std140 array in a struct with a single `data` member, so an
     * array element arrives as "u_arr.data[3]". Fold that back to "u_arr[3]". */
    for (auto pos = name.find(".data["); pos != std::string::npos; pos = name.find(".data[", pos))
        name.erase(pos, 5);

    return name;
}

std::vector<UniformBlock> UniformBlock::discoverAll(const uint32_t program) {
    int blockCount = 0;
    GLC(glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &blockCount));

    std::vector<UniformBlock> blocks;
    blocks.reserve(static_cast<std::size_t>(blockCount));

    for (int index = 0; index < blockCount; ++index) {
        int dataSize = 0;
        GLC(glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_DATA_SIZE, &dataSize));

        UniformBlock block;
        block.m_bindingPoint = static_cast<uint32_t>(index);
        block.m_data.assign(static_cast<std::size_t>(dataSize), 0);

        GLC(glGenBuffers(1, &block.m_buffer));
        GLC(glBindBuffer(GL_UNIFORM_BUFFER, block.m_buffer));
        GLC(glBufferData(GL_UNIFORM_BUFFER, dataSize, nullptr, GL_DYNAMIC_DRAW));

        // Each block gets its own binding point. bind() reasserts it every frame.
        GLC(glUniformBlockBinding(program, index, block.m_bindingPoint));

        int memberCount = 0;
        GLC(glGetActiveUniformBlockiv(program, index,
            GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &memberCount));

        if (memberCount > 0) {
            const auto count = static_cast<std::size_t>(memberCount);
            std::vector<int> signedIndices(count);
            GLC(glGetActiveUniformBlockiv(program, index,
                GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, signedIndices.data()));

            const std::vector<unsigned int> indices(signedIndices.begin(), signedIndices.end());
            std::vector<int> offsets(count);
            std::vector<int> sizes(count);
            std::vector<int> arrayStrides(count);
            GLC(glGetActiveUniformsiv(program, memberCount, indices.data(), GL_UNIFORM_OFFSET, offsets.data()));
            GLC(glGetActiveUniformsiv(program, memberCount, indices.data(), GL_UNIFORM_SIZE, sizes.data()));
            GLC(glGetActiveUniformsiv(program, memberCount, indices.data(), GL_UNIFORM_ARRAY_STRIDE, arrayStrides.data()));

            for (std::size_t i = 0; i < count; ++i) {
                char nameBuffer[512] = {};
                int length = 0;
                GLC(glGetActiveUniformName(program, indices[i],
                    static_cast<int>(sizeof(nameBuffer)), &length, nameBuffer));

                const std::string memberName =
                    normalizeName(std::string(nameBuffer, static_cast<std::size_t>(length)));

                /* Arrays of scalars, vectors or matrices are reported once, as "name[0]" with
                 * a size and a stride. Arrays of structs are already enumerated per element. */
                const bool isArray = sizes[i] > 1 && memberName.size() > 3
                    && memberName.compare(memberName.size() - 3, 3, "[0]") == 0;
                if (isArray) {
                    const std::string base = memberName.substr(0, memberName.size() - 3);
                    for (int element = 0; element < sizes[i]; ++element)
                        block.m_offsets[base + "[" + std::to_string(element) + "]"] = offsets[i] + element * arrayStrides[i];
                } else {
                    block.m_offsets[memberName] = offsets[i];
                }
            }
        }
        blocks.push_back(std::move(block));
    }
    return blocks;
}

UniformBlock::~UniformBlock() {
    destroy();
}

UniformBlock::UniformBlock(UniformBlock&& other) noexcept
    : m_buffer(std::exchange(other.m_buffer, 0)),
      m_bindingPoint(other.m_bindingPoint),
      m_data(std::move(other.m_data)),
      m_dirty(other.m_dirty),
      m_offsets(std::move(other.m_offsets)) {}

UniformBlock& UniformBlock::operator=(UniformBlock&& other) noexcept {
    if (this != &other) {
        destroy();
        m_buffer = std::exchange(other.m_buffer, 0);
        m_bindingPoint = other.m_bindingPoint;
        m_data = std::move(other.m_data);
        m_dirty = other.m_dirty;
        m_offsets = std::move(other.m_offsets);
    }
    return *this;
}

void UniformBlock::destroy() noexcept {
    // No GLC on purpose, a dying context makes every call fail, and a destructor must not throw.
    if (m_buffer != 0) {
        glDeleteBuffers(1, &m_buffer);
        m_buffer = 0;
    }
}

bool UniformBlock::holds(const std::string& member) const {
    return m_offsets.find(member) != m_offsets.end();
}

bool UniformBlock::write(const std::string& member, const UniformValue& value) {
    const auto it = m_offsets.find(member);
    if (it == m_offsets.end())
        return false;

    uint8_t* destination = m_data.data() + it->second;

    // Only mark dirty when the bytes really change, so an unmoved parameter costs nothing.
    const auto store = [&](const void* source, const std::size_t size) {
        if (std::memcmp(destination, source, size) == 0)
            return;
        std::memcpy(destination, source, size);
        m_dirty = true;
    };

    std::visit([&]<typename T0>(T0&& val) {
        using T = std::decay_t<T0>;
        if constexpr (std::is_same_v<T, float> || std::is_same_v<T, int>) {
            store(&val, sizeof(T));
        } else if constexpr (std::is_same_v<T, Vec2<float>>) {
            const float packed[2] = {val.x, val.y};
            store(packed, sizeof(packed));
        } else if constexpr (std::is_same_v<T, Vec3<float>>) {
            const float packed[3] = {val.x, val.y, val.z};
            store(packed, sizeof(packed));
        } else if constexpr (std::is_same_v<T, Vec4<float>>) {
            const float packed[4] = {val.x, val.y, val.z, val.w};
            store(packed, sizeof(packed));
        } else if constexpr (std::is_same_v<T, Vec4<int>>) {
            const int packed[4] = {val.x, val.y, val.z, val.w};
            store(packed, sizeof(packed));
        } else if constexpr (std::is_same_v<T, Mat4>) {
            // std140 gives a mat4 a 16-byte vector stride, so its 64 bytes are contiguous.
            store(val.m, sizeof(val.m));
        }
    }, value);

    return true;
}

void UniformBlock::bind() {
    /* Rebind every time because glBindBufferBase is context state, not program state, so another
     * shader using the same binding point would otherwise leave ours pointing at its
     * buffer. */
    GLC(glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint, m_buffer));

    if (m_dirty) {
        GLC(glBindBuffer(GL_UNIFORM_BUFFER, m_buffer));
        GLC(glBufferSubData(GL_UNIFORM_BUFFER, 0,
            static_cast<GLsizeiptr>(m_data.size()), m_data.data()));
        m_dirty = false;
    }
}
