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

#include <string>

namespace shimera {

/**
 * Where the built-in post-processing shaders live.
 *
 * POC ONLY. SHIMERA_RES_DIR is baked in by xmake and points at the source tree, which
 * is fine while the only consumers are the examples in this repository and not for
 * anything shipped. The final goal would be to use GLShader::fromMemory() with the
 * shaders embedded in the binary. For now, I'll use that.
 */
inline std::string builtinShader(const std::string& fileName) {
    return std::string(SHIMERA_RES_DIR) + "/shader/postprocessing/" + fileName;
}

inline std::string postProcessVertex() {
    return builtinShader("postprocess.vert");
}

// Slang generated output
inline std::string generatedShader(const std::string& fileName) {
    return std::string(SHIMERA_RES_DIR) + "/shader/generated/" + fileName;
}

// Materials shade geometry rather than reprocess an image, so they live beside, not under.
inline std::string materialShader(const std::string& fileName) {
    return std::string(SHIMERA_RES_DIR) + "/shader/material/" + fileName;
}

}
