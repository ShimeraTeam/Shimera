#pragma once

#include <shimera_api_export.h>
#include <optional>
#include <string>

namespace shimera {

/** Reads a whole text file, and return its content as a string,
 * or `std::nullopt` if an error occurred (file does not exist). */
EXPORT [[nodiscard]] std::optional<std::string> readTextFile(const std::string& path);

}
