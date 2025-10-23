cmake_minimum_required(VERSION 3.16)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    set(BUILD_DIR "${CMAKE_SOURCE_DIR}/build")
    file(MAKE_DIRECTORY "${BUILD_DIR}")
    message(STATUS "Configuration automatique dans ${BUILD_DIR}/")
    execute_process(
        COMMAND ${CMAKE_COMMAND} 
            -S "${CMAKE_SOURCE_DIR}"
            -B "${BUILD_DIR}"
            ${CMAKE_ARGS}
        RESULT_VARIABLE result
    )
    if(result EQUAL 0)
        message(STATUS "\n✓ Configuration réussie dans build/")
        message(STATUS "Pour compiler :")
        message(STATUS "  cd build && make -j$(nproc)\n")
    endif()
    return()
endif()

project(hello)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(SDL3)

add_executable(sdl_project WIN32 main.cpp)

target_link_libraries(sdl_project PRIVATE SDL3::SDL3)