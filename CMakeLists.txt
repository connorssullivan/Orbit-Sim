cmake_minimum_required(VERSION 3.10)
project(HelloWindow)

# === C++ Standard ===
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================
# Assimp (submodule)
# ============================================================
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(assimp)

# ============================================================
# GLFW (submodule)
# ============================================================
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)

# ============================================================
# OpenGL
# ============================================================
find_package(OpenGL REQUIRED)

# ============================================================
# Executable
# ============================================================
add_executable(main
    src/main.cpp
    src/glad.c
    src/callbacks.cpp
    src/window.cpp
    src/vertexArray.cpp
    src/vertexBuffer.cpp
    src/indexBuffer.cpp
    src/renderer.cpp 
    src/shader.cpp 
    src/stb_image.cpp
    src/backGroundTexture.cpp
    src/model.cpp
    src/planet.cpp
)

# ============================================================
# Include directories
# ============================================================
target_include_directories(main PRIVATE
    ${GLFW_PATH}/include
    ${CMAKE_SOURCE_DIR}/include   # glad/, stb_image.h, your headers
    ${CMAKE_SOURCE_DIR}           # glm/
    ${CMAKE_SOURCE_DIR}/include/textures 
    ${CMAKE_SOURCE_DIR}/include/general 
    ${CMAKE_SOURCE_DIR}/include/vertex
    ${CMAKE_SOURCE_DIR}/include/model  
    ${CMAKE_SOURCE_DIR}/include/shaders
)

# ============================================================
# Link libraries
# ============================================================
target_link_libraries(main
    assimp
    glfw
    OpenGL::GL
)

# ============================================================
# macOS frameworks
# ============================================================
if (APPLE)
    target_link_libraries(main
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework CoreGraphics"
    )
endif()
