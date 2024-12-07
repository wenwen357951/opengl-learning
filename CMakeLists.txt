# CMake
cmake_minimum_required(VERSION 3.5)

# Project
project(opengl_learn_project VERSION 0.1.0)

# GLFW
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(external/glfw)

# Glad
include_directories(SYSTEM "external/glad/include")

# Sources
file(GLOB opengl_learn_project_cpp_sources src/*.cpp)
set(
    opengl_learn_project_src
    ${opengl_learn_project_cpp_sources}
    "external/glad/src/glad.c"
)

# Build
add_executable(${PROJECT_NAME} ${opengl_learn_project_src})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw)
