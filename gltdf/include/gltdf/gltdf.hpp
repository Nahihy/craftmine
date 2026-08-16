#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

#include "../../src/gltdf/shader.hpp"
#include "../../src/gltdf/window.hpp"
#include "../../src/gltdf/framebuffer.hpp"

namespace gltdf {

  int initWindowSystem() {
    if(!glfwInit()) {
      std::cout << "Failed to initialize GLFW\n";
      return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    return 0;
  }

  int initGraphicSystem() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
    }
    return 0;
  }

  void inline terminate() {
    glfwTerminate();  
  }

}
