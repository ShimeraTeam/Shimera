cmake_minimum_required(VERSION 3.16)
project(SFML_Example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download and build SFML 3 automatically
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
)
FetchContent_MakeAvailable(SFML)

# Create executable
add_executable(sfml_example main.cpp)

# Link SFML libraries
target_link_libraries(sfml_example PRIVATE sfml-graphics sfml-window sfml-system)

# Copy SFML DLLs to build directory (for Windows, does nothing on Linux)
if(WIN32)
    add_custom_command(
        TARGET sfml_example
        COMMENT "Copy SFML DLLs"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SFML_BINARY_DIR}/lib
            $<TARGET_FILE_DIR:sfml_example>
        VERBATIM
    )
endif()
