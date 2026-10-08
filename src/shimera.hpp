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

#include "Context.hpp"
#include "EffectPipeline.hpp"
#include "Error.hpp"
#include "GL/GLFramebuffer.hpp"
#include "GL/GLStateGuard.hpp"
#include "GL/GLMesh.hpp"
#include "GL/GLTexture.hpp"
#include "common/Color.inl"
#include "effects/Brightness.hpp"
#include "effects/Contrast.hpp"
#include "effects/Distortion.hpp"
#include "effects/Pixelisation.hpp"
#include "effects/Bloom.hpp"
#include "effects/Saturation.hpp"
#include "effects/Vignette.hpp"
#include "effects/ChromaticAberration.hpp"
#include "effects/GaussianBlur.hpp"
#include "materials/Fresnel.hpp"
#include "scene/Camera.hpp"
#include "scene/Transform.hpp"
