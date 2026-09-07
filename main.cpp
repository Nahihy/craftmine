#include "glm/detail/qualifier.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <cstdio>
#include <gltdf/gltdf.hpp>
#include <gl2df/gl2df.hpp>
#include "player.hpp"
#include <iostream>
#include <unordered_map>
#include "world.hpp"
#include "skybox.hpp"

void processInput(const gltdf::Window& window) {
  if (glfwGetKey(window.glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
    ((gl3df::Camera*)(window.userPtr))->processKeyboard(gl3df::FORWARD, window.deltaTime);
  if (glfwGetKey(window.glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
    ((gl3df::Camera*)(window.userPtr))->processKeyboard(gl3df::BACKWARD, window.deltaTime);
  if (glfwGetKey(window.glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
    ((gl3df::Camera*)(window.userPtr))->processKeyboard(gl3df::LEFT, window.deltaTime);
  if (glfwGetKey(window.glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
    ((gl3df::Camera*)(window.userPtr))->processKeyboard(gl3df::RIGHT, window.deltaTime);
  if (glfwGetKey(window.glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    ((gl3df::Camera*)(window.userPtr))->processKeyboard(gl3df::UP, window.deltaTime);
  if (glfwGetKey(window.glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    ((gl3df::Camera*)(window.userPtr))->processKeyboard(gl3df::DOWN, window.deltaTime);
}


int main() {

  gltdf::initWindowSystem();

  gltdf::Window window("craftmine", 600, 800);
  window.makeCurrent();
  window.setAutoResizeFrameBuffer();
  glfwSetInputMode(window.glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  gl3df::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f),
             gl3df::YAW, gl3df::PITCH, gl3df::SPEED * 5, gl3df::SENSITIVITY, gl3df::ZOOM);


  window.setCustomUserPtr(&camera);
  window.setAutoOffsetCalcMouseCallback([](gltdf::Window* window, double xoffset, double yoffset)
                                        {((gl3df::Camera*)(window->userPtr))->processMouseMovement(xoffset, yoffset);});
  
  window.setScollOffsetCallback([](gltdf::Window* window, double xoffset, double yoffset)
                                        {((gl3df::Camera*)(window->userPtr))->processMouseScroll(yoffset);});

  gltdf::initGraphicSystem();

  Player player;

  World world;
  world.chunks[glm::ivec3(3, 3, 3)];
  world.chunks[glm::ivec3(2, 1, 4)];
  world.chunks[glm::ivec3(8, 2, 6)];
  world.chunks[glm::ivec3(2, 2, 2)];
  world.chunks[glm::ivec3(3, 2, 3)];

  SkyBox skybox;

  player.bindToGeneralUBO(world.blockShader, "vp");
  player.bindToskyboxUBO(skybox.shader, "vp");

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);


  while(!window.shouldClose()) {
    window.updateDeltaTime();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    processInput(window);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)window.width / (float)window.height, 0.1f, 1000.0f);
      
    player.updateUBO(view, projection);
    world.draw();
   
    skybox.draw();

    window.swapAndPollEvents();
  }


  gltdf::terminate();
  return 0;
}
