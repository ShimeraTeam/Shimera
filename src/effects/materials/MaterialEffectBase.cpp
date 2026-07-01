#include "effects/materials/MaterialEffectBase.hpp"

#include "scene/TransformFactory.hpp"

void shimera::MaterialEffectBase::setEnabled(const bool enabled) {
    m_enabled = enabled;
}

bool shimera::MaterialEffectBase::isEnabled() const {
    return m_enabled;
}

void shimera::MaterialEffectBase::setTransform(const Vec3<float>& position,
        const Vec3<float>& rotationEuler, const Vec3<float>& scale) {
    m_transform = TransformFactory::model(position, rotationEuler, scale);
}
