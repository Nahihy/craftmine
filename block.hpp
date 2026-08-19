#include "gltdf/src/gl2df/sprite.hpp"
#include "gltdf/src/gl2df/vertexArray.hpp"
#include <cstdio>
#include <gltdf/gltdf.hpp>
#include <gl2df/gl2df.hpp>
#include <gl3df/gl3df.hpp>

#define BLOCK_VERTICES {\
    -1.0f,  1.0f, -1.0f,   0.0f, 1.0f,\
    -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,\
     1.0f, -1.0f, -1.0f,   1.0f, 0.0f,\
     1.0f, -1.0f, -1.0f,   1.0f, 0.0f,\
     1.0f,  1.0f, -1.0f,   1.0f, 1.0f,\
    -1.0f,  1.0f, -1.0f,   0.0f, 1.0f,\
    -1.0f, -1.0f,  1.0f,   0.0f, 1.0f,\
    -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,\
    -1.0f,  1.0f, -1.0f,   1.0f, 0.0f,\
    -1.0f,  1.0f, -1.0f,   1.0f, 0.0f,\
    -1.0f,  1.0f,  1.0f,   1.0f, 1.0f,\
    -1.0f, -1.0f,  1.0f,   0.0f, 1.0f,\
     1.0f, -1.0f, -1.0f,   0.0f, 0.0f,\
     1.0f, -1.0f,  1.0f,   0.0f, 1.0f,\
     1.0f,  1.0f,  1.0f,   1.0f, 1.0f,\
     1.0f,  1.0f,  1.0f,   1.0f, 1.0f,\
     1.0f,  1.0f, -1.0f,   1.0f, 0.0f,\
     1.0f, -1.0f, -1.0f,   0.0f, 0.0f,\
    -1.0f, -1.0f,  1.0f,   0.0f, 0.0f,\
    -1.0f,  1.0f,  1.0f,   0.0f, 1.0f,\
     1.0f,  1.0f,  1.0f,   1.0f, 1.0f,\
     1.0f,  1.0f,  1.0f,   1.0f, 1.0f,\
     1.0f, -1.0f,  1.0f,   1.0f, 0.0f,\
    -1.0f, -1.0f,  1.0f,   0.0f, 0.0f,\
    -1.0f,  1.0f, -1.0f,   0.0f, 0.0f,\
     1.0f,  1.0f, -1.0f,   1.0f, 0.0f,\
     1.0f,  1.0f,  1.0f,   1.0f, 1.0f,\
     1.0f,  1.0f,  1.0f,   1.0f, 1.0f,\
    -1.0f,  1.0f,  1.0f,   0.0f, 1.0f,\
    -1.0f,  1.0f, -1.0f,   0.0f, 0.0f,\
    -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,\
    -1.0f, -1.0f,  1.0f,   0.0f, 1.0f,\
     1.0f, -1.0f, -1.0f,   1.0f, 0.0f,\
     1.0f, -1.0f, -1.0f,   1.0f, 0.0f,\
    -1.0f, -1.0f,  1.0f,   0.0f, 1.0f,\
     1.0f, -1.0f,  1.0f,   1.0f, 1.0f\
  }

class Block {

  public:
    gl2df::Sprite sprite;
    std::vector<glm::mat4> modelList;

    Block(const char* texName, std::vector<glm::mat4> modelList = {glm::mat4(1.0f)})
      : sprite(gltdf::Shader("block/vertex.glsl", "block/fragment.glsl"),
               gl2df::VertexArray(BLOCK_VERTICES, {}, {{0, 3, 5 * sizeof(float), 0}, {1, 2, 5 * sizeof(float), 3 * sizeof(float)}}), 
               {{gl2df::Texture(texName)}}), modelList(modelList) {}

    void draw() {
      char buff[32];
      for(int i = 0; i < this->modelList.size(); i++) {
        snprintf(buff, sizeof(buff), "models[%d]", i);
        this->sprite.shader.setMat4(buff, this->modelList[i]);
      }
      sprite.instanceDraw(this->modelList.size());
    }
  


};
