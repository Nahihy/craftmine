#include "glm/detail/qualifier.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <cstdio>
#include <gltdf/gltdf.hpp>
#include <gl2df/gl2df.hpp>
#include "player.hpp"
#include <iostream>
#include <unordered_map>
#include "world.hpp"

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


unsigned int loadCubemap(std::vector<std::string> faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
  int width, height, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++) {
    stbi_set_flip_vertically_on_load(false);
    unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      GLenum format = GL_RGB;
      if(nrChannels == 1) format = GL_RED;
      else if(nrChannels == 2) format = GL_RG;
      else if(nrChannels == 3) format = GL_RGB;
      else if(nrChannels == 4) format = GL_RGBA;
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                   0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
      );
      stbi_image_free(data);
    } else {
      std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
      stbi_image_free(data);
    }
}
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
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


  std::vector<std::string> skyboxFaces = {
    "assets/textures/skybox/right.jpg",
    "assets/textures/skybox/left.jpg",
    "assets/textures/skybox/top.jpg",
    "assets/textures/skybox/bottom.jpg",
    "assets/textures/skybox/front.jpg",
    "assets/textures/skybox/back.jpg"
  };

  unsigned int skyboxTexture = loadCubemap(skyboxFaces); 

  std::vector<float> skyboxVertices = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
  };

  gl2df::VertexArray skyboxVertexArr(skyboxVertices, {}, {{0, 3, 3 * sizeof(float), 0}});

  gltdf::Shader skyboxShader("skybox/vertex.glsl", "skybox/fragment.glsl");

  Player player;

  World world;
  world.chunks[glm::ivec3(3, 3, 3)];
  world.chunks[glm::ivec3(2, 1, 4)];
  world.chunks[glm::ivec3(8, 2, 6)];
  world.chunks[glm::ivec3(2, 2, 2)];
  world.chunks[glm::ivec3(3, 2, 3)];


  player.bindToGeneralUBO(world.blockShader, "vp");
  player.bindToskyboxUBO(skyboxShader, "vp");

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

    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glDepthMask(GL_FALSE);
    skyboxShader.bind();
    skyboxVertexArr.draw();
    glDepthMask(GL_TRUE);

    window.swapAndPollEvents();
  }


  gltdf::terminate();
  return 0;
}
