#include "RaylibTarget.hpp"

#include <rlgl.h>

#include <stdexcept>
#include <string>

using shimera::RaylibTarget;

RaylibTarget::RaylibTarget(const int width, const int height, const bool samplableDepth)
    : m_samplableDepth(samplableDepth) {
    RaylibTarget::resize(width, height);
}

RaylibTarget::~RaylibTarget() {
    UnloadRenderTexture(m_renderTexture);
}

void RaylibTarget::begin() {
    BeginTextureMode(m_renderTexture);
}

void RaylibTarget::end() {
    rlDrawRenderBatchActive();
    EndTextureMode();
}

void RaylibTarget::resize(const int width, const int height) {
    m_width = width;
    m_height = height;

    UnloadRenderTexture(m_renderTexture);
    m_renderTexture = m_samplableDepth ? makeWithDepthTexture(width, height)
                                       : LoadRenderTexture(width, height);

    if (m_renderTexture.texture.id == 0)
        throw std::runtime_error("shimera: RaylibTarget could not resize to "
            + std::to_string(width) + "x" + std::to_string(height));

    m_color.emplace(m_renderTexture.texture.id, width, height);
    if (m_samplableDepth)
        m_depth.emplace(m_renderTexture.depth.id, width, height);
}

shimera::Camera RaylibTarget::cameraFromRaylib(const Vec3<float>& eyePosition) {
    const auto toMat4 = [](const Matrix& m) {
        return Mat4{{m.m0, m.m1, m.m2, m.m3,
                     m.m4, m.m5, m.m6, m.m7,
                     m.m8, m.m9, m.m10, m.m11,
                     m.m12, m.m13, m.m14, m.m15}};
    };
    return Camera::fromMatrices(toMat4(rlGetMatrixModelview()),
                                toMat4(rlGetMatrixProjection()), eyePosition);
}

RenderTexture2D RaylibTarget::makeWithDepthTexture(const int width, const int height) {
    RenderTexture2D target = {0};

    target.id = rlLoadFramebuffer();
    rlEnableFramebuffer(target.id);

    target.texture.id = rlLoadTexture(nullptr, width, height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
    target.texture.width = width;
    target.texture.height = height;
    target.texture.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    target.texture.mipmaps = 1;

    target.depth.id = rlLoadTextureDepth(width, height, false);
    target.depth.width = width;
    target.depth.height = height;

    rlFramebufferAttach(target.id, target.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
    rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);
    rlDisableFramebuffer();

    return target;
}
