#include <cstdio>
#include <cstdlib>
#include <gltdf/gltdf.hpp>
#include <gl2df/gl2df.hpp>
#include "player.hpp"
#include "world.hpp"
#include "skybox.hpp"
#include "fastnoiseLite.hpp"


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

  gl3df::Camera camera(glm::vec3(0.0f, 256.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
             gl3df::YAW, gl3df::PITCH, gl3df::SPEED * 10, gl3df::SENSITIVITY, gl3df::ZOOM);


  window.setCustomUserPtr(&camera);
  window.setAutoOffsetCalcMouseCallback([](gltdf::Window* window, double xoffset, double yoffset)
                                        {((gl3df::Camera*)(window->userPtr))->processMouseMovement(xoffset, yoffset);});
  
  window.setScollOffsetCallback([](gltdf::Window* window, double xoffset, double yoffset)
                                        {((gl3df::Camera*)(window->userPtr))->processMouseScroll(yoffset);});

  gltdf::initGraphicSystem();

  Player player;

  World world;

  SkyBox skybox;

  player.bindToGeneralUBO(world.blockShader, "vp");
  player.bindToskyboxUBO(skybox.shader, "vp");


  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  while(!window.shouldClose()) {
    window.updateDeltaTime();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    processInput(window);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)window.width / (float)window.height, 0.1f, 1000.0f);
      
    player.updateUBO(view, projection);
    world.draw(glm::ivec2(camera.position.x, camera.position.z));
   
    skybox.draw();

    // std::printf("(%.2f,%.2f,%.2f)\n", camera.position.x, camera.position.y, camera.position.z);
    // std::cout << window.deltaTime << std::endl;

    window.swapAndPollEvents();
  }


  gltdf::terminate();
  return 0;
}
