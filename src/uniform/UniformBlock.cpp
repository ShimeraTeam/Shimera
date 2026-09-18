#include "UniformBlock.hpp"

#include <GL/glew.h>
#include "GL/GLError.hpp"

#include <cstring>
#include <utility>
#include <variant>

#include "Vec2.inl"
#include "Vec3.inl"
#include "Vec4.inl"
#include "Mat4.hpp"

using shimera::UniformBlock;

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
            std::vector<int> signedIndices(static_cast<std::size_t>(memberCount));
            GLC(glGetActiveUniformBlockiv(program, index,
                GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, signedIndices.data()));

            const std::vector<unsigned int> indices(signedIndices.begin(), signedIndices.end());
            std::vector<int> offsets(static_cast<std::size_t>(memberCount));
            GLC(glGetActiveUniformsiv(program, memberCount, indices.data(),
                GL_UNIFORM_OFFSET, offsets.data()));

            for (int i = 0; i < memberCount; ++i) {
                char nameBuffer[256] = {};
                int length = 0;
                GLC(glGetActiveUniformName(program, indices[i],
                    static_cast<int>(sizeof(nameBuffer)), &length, nameBuffer));

                std::string memberName(nameBuffer, static_cast<std::size_t>(length));
                /* GL reports "BlockName.member" when the block has an instance name, which
                 * Slang's output always does. Effects know the bare name, so store that. */
                if (const auto dot = memberName.rfind('.'); dot != std::string::npos)
                    memberName = memberName.substr(dot + 1);

                block.m_offsets[memberName] = offsets[i];
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
        } else if constexpr (std::is_same_v<T, Mat4>) {
            // std140 gives a mat4 a 16-byte column stride, so its 64 bytes are contiguous.
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
