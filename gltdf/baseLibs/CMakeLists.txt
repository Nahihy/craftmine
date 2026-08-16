
find_package(assimp)
find_package(glfw)


if(NOT glfw_FOUND)
  add_subdirectory(glfw)
endif()
if(NOT assimp_FOUND)
  add_subdirectory(assimp)
endif()

add_subdirectory(glad)
add_subdirectory(stb)
add_subdirectory(glm)
