#pragma once

#include <stdexcept>
#include <string>

namespace shimera {

// Thrown by Context::create() when the library cannot come up.
class InitError final : public std::runtime_error {
    public:
        explicit InitError(const std::string& message) : std::runtime_error(message) {}
};

}
