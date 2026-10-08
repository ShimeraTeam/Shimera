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

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "UniformValue.inl"

namespace shimera {

/**
 * One std140 uniform block of a linked program, with its GPU buffer and a CPU mirror.
 *
 * Writes go to the mirror and are compared before being stored, so a block only reaches the
 * GPU when its bytes actually changed. Effects re-send every parameter every frame and most
 * never move, which turns a guaranteed upload per frame into an upload per real change
 * (Similar to what is done for regular Uniforms).
 */
class UniformBlock {
    public:
        // Every block of a linked program, each bound to a binding point equal to its index.
        static std::vector<UniformBlock> discoverAll(uint32_t program);

        UniformBlock() = default;
        ~UniformBlock();

        UniformBlock(const UniformBlock&) = delete;
        UniformBlock& operator=(const UniformBlock&) = delete;
        UniformBlock(UniformBlock&& other) noexcept;
        UniformBlock& operator=(UniformBlock&& other) noexcept;

        bool write(const std::string& member, const UniformValue& value);
        void bind();

        [[nodiscard]] bool holds(const std::string& member) const;
        [[nodiscard]] std::size_t byteSize() const { return m_data.size(); }
        [[nodiscard]] uint32_t bindingPoint() const { return m_bindingPoint; }

    private:
        void destroy() noexcept;

        uint32_t m_buffer = 0;
        uint32_t m_bindingPoint = 0;
        std::vector<uint8_t> m_data; // CPU mirror, uploaded when dirty
        bool m_dirty = false;
        std::unordered_map<std::string, int> m_offsets; // member name to byte offset
};

}
