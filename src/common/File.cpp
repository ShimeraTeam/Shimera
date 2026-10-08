#include "File.hpp"

#include <fstream>
#include <sstream>

std::optional<std::string> shimera::readTextFile(const std::string& path) {
    const std::ifstream stream(path);
    if (!stream.is_open())
        return std::nullopt;

    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}
