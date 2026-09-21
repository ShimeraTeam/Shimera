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
