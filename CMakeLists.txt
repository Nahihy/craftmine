cmake_minimum_required(VERSION 3.14)

project(craftmine)

set(SHADER_DIR "${CMAKE_SOURCE_DIR}/assets/shaders" CACHE STRING "Shader directory")
set(TEXTURE_DIR "${CMAKE_SOURCE_DIR}/assets/textures" CACHE STRING "Texture directory")
set(MODEL_DIR "${CMAKE_SOURCE_DIR}/assets/models" CACHE STRING "Model directory")

find_package(OpenGL REQUIRED)

add_subdirectory(gltdf)

add_executable(craftmine main.cpp player.hpp world.hpp skybox.hpp)

target_link_libraries(craftmine gltdf OpenGL::GL glad glfw stb glm assimp)
