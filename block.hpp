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
    std::vector<glm::vec3> modelList;
    unsigned int modelVBO;

    Block(const char* texName, std::vector<glm::vec3> modelList = {})
      : sprite(gltdf::Shader("block/vertex.glsl", "block/fragment.glsl"),
               gl2df::VertexArray(BLOCK_VERTICES, {}, {{0, 3, 5 * sizeof(float), 0}, {1, 2, 5 * sizeof(float), 3 * sizeof(float)}}), 
               {{gl2df::Texture(texName)}}), modelList(modelList) {
      this->sprite.vertArr.bind();
      glGenBuffers(1, &this->modelVBO);
      glBindBuffer(GL_ARRAY_BUFFER, this->modelVBO);
      glBufferData(GL_ARRAY_BUFFER, this->modelList.size() * sizeof(glm::vec3), this->modelList.data(), GL_DYNAMIC_DRAW);
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
      glVertexAttribDivisor(2, 1);
      glEnableVertexAttribArray(2);
    }

    void draw() {
      sprite.instanceDraw(this->modelList.size());
    }
  


};
