cmake_minimum_required(VERSION 3.14)

if(NOT DEFINED SHADER_DIR)
  set(SHADER_DIR "${CMAKE_SOURCE_DIR}/assets/shaders" CACHE STRING "Shader directory")
  message("SHADER_DIR not set! Setting as ${SHADER_DIR}")
endif()

if(NOT DEFINED TEXTURE_DIR)
  set(TEXTURE_DIR "${CMAKE_SOURCE_DIR}/asssts/textures" CACHE STRING "Texture directory")
  message("TEXTURE_DIR not set! Setting as ${TEXTURE_DIR}")
endif()

if(NOT DEFINED MODEL_DIR)
  set(MODEL_DIR "${CMAKE_SOURCE_DIR}/assets/models" CACHE STRING "Model directory")
  message("MODEL_DIR not set! Setting as ${MODEL_DIR}")
endif()

set(SHADER_BINARY_DIR "assets/shaders" CACHE STRING "Shader directory in binary dir")
set(TEXTURE_BINARY_DIR "assets/textures" CACHE STRING "Texture directory in binary dir")
set(MODEL_BINARY_DIR "assets/models" CACHE STRING "Model directory in binary dir")

project(gltdf)

find_package(OpenGL REQUIRED)

add_library(gltdf 

  src/gltdf/shader.hpp
  src/gltdf/window.hpp
  src/gltdf/framebuffer.hpp

  src/gl3df/mesh.hpp
  src/gl3df/camera.hpp
  src/gl3df/model.hpp

  src/gl2df/vertexArray.hpp
  src/gl2df/texture.hpp
  src/gl2df/sprite.hpp

)

add_subdirectory(baseLibs)

target_link_libraries(gltdf PUBLIC assimp glm glad glfw stb OpenGL::GL)
target_include_directories(gltdf PUBLIC include)

target_compile_definitions(gltdf PUBLIC
  SHADER_BINARY_DIR="${SHADER_BINARY_DIR}/"
  TEXTURE_BINARY_DIR="${TEXTURE_BINARY_DIR}/"
  MODEL_BINARY_DIR="${MODEL_BINARY_DIR}/"
)

add_custom_target(copy-shaders-dir ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_DIR} ${CMAKE_BINARY_DIR}/${SHADER_BINARY_DIR}
)
add_custom_target(copy-texture-dir ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${TEXTURE_DIR} ${CMAKE_BINARY_DIR}/${TEXTURE_BINARY_DIR}
)
add_custom_target(copy-model-dir ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${MODEL_DIR} ${CMAKE_BINARY_DIR}/${MODEL_BINARY_DIR}
)

