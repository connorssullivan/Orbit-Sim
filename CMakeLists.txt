cmake_minimum_required(VERSION 3.10)

# GLFW builds Objective-C on macOS; GCC cannot parse Apple SDK headers (nullable, etc.).
# Conda/Homebrew often set CC/CXX to GCC — switch to Apple Clang before compiler detection.
if(APPLE)
    if(DEFINED ENV{CC} AND "$ENV{CC}" MATCHES "gcc")
        set(CMAKE_C_COMPILER "/usr/bin/clang" CACHE FILEPATH "C compiler" FORCE)
    endif()
    if(DEFINED ENV{CXX} AND "$ENV{CXX}" MATCHES "g\\+\\+")
        set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE FILEPATH "C++ compiler" FORCE)
    endif()
endif()

project(HelloWindow)

if(APPLE)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU")
        message(FATAL_ERROR
            "This project must be built with Apple Clang on macOS (not GCC). "
            "GLFW's Cocoa backend compiles Objective-C against the system SDK. "
            "Reconfigure from a clean build dir, for example:\n"
            "  CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -B build -S ."
        )
    endif()
endif()

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

find_package(assimp REQUIRED)

# Homebrew assimpConfig may pin INTERFACE_LINK_LIBRARIES to libz.tbd under a specific
# SDK path; if that SDK is not present, linking fails. Prefer CMake's zlib target.
find_package(ZLIB REQUIRED)
if(TARGET assimp::assimp)
    get_target_property(_assimp_iface_libs assimp::assimp INTERFACE_LINK_LIBRARIES)
    if(_assimp_iface_libs MATCHES "\\.tbd")
        set_target_properties(assimp::assimp PROPERTIES INTERFACE_LINK_LIBRARIES ZLIB::ZLIB)
    endif()
endif()

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

    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp

)

# Absolute asset root so shaders/models/textures load regardless of current working directory
target_compile_definitions(main PRIVATE "ORBIT_SIM_ROOT=\"${CMAKE_SOURCE_DIR}\"")

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

    # IAM Gui
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/imgui/backends
)

# ============================================================
# Link libraries
# ============================================================
target_link_libraries(main
    assimp::assimp
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
