#include "effects/ShaderEffectBase.hpp"

using shimera::IPostProcessor;
using shimera::ShaderEffectBase;

void ShaderEffectBase::setEnabled(bool enabled) {
    m_enabled = enabled;
}

bool ShaderEffectBase::isEnabled() const {
    return m_enabled;
}

bool ShaderEffectBase::isDepthNeeded() const {
    return false;
}

void ShaderEffectBase::setDepthTexture(ITexture& depth) {}

IPostProcessor* ShaderEffectBase::getPostProcessor() const {
    return m_processor.get();
}
