#include "effects/ShaderEffectBase.hpp"

void ShaderEffectBase::setEnabled(bool enabled) {
    m_enabled = enabled;
}

bool ShaderEffectBase::isEnabled() const {
    return m_enabled;
}

IPostProcessor* ShaderEffectBase::getPostProcessor() const {
    return m_processor.get();
}
