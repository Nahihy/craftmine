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
    std::vector<glm::ivec3> posList;
    unsigned int posVBO;
    std::size_t posVBOcapacity;

    Block(const char* texName, const std::vector<glm::ivec3>& posList = {})
      : sprite(gltdf::Shader("block/vertex.glsl", "block/fragment.glsl"),
               gl2df::VertexArray(BLOCK_VERTICES, {}, {{0, 3, 5 * sizeof(float), 0}, {1, 2, 5 * sizeof(float), 3 * sizeof(float)}}), 
               {{gl2df::Texture(texName)}}), posList(posList) {
      this->sprite.vertArr.bind();
      this->posVBOcapacity = posList.size();
      glGenBuffers(1, &this->posVBO);
      glBindBuffer(GL_ARRAY_BUFFER, this->posVBO);
      glBufferData(GL_ARRAY_BUFFER, this->posVBOcapacity * sizeof(glm::ivec3), this->posList.data(), GL_DYNAMIC_DRAW);
      glVertexAttribIPointer(2, 3, GL_INT, sizeof(glm::vec3), (void*)0);
      glVertexAttribDivisor(2, 1);
      glEnableVertexAttribArray(2);
      glBindVertexArray(0);
    }

    void add(const glm::ivec3& pos) {
      this->posList.push_back(pos);
      if(this->posVBOcapacity >= this->posList.size()) {
        glBindBuffer(GL_ARRAY_BUFFER, this->posVBO);
        glBufferSubData(GL_ARRAY_BUFFER, (this->posList.size() - 1) * sizeof(glm::ivec3),
                        sizeof(glm::ivec3), &this->posList.back());
      } else {
        do this->posVBOcapacity *= 2;
        while(this->posVBOcapacity < this->posList.size());
        glBindBuffer(GL_ARRAY_BUFFER, this->posVBO);
        glBufferData(GL_ARRAY_BUFFER, this->posVBOcapacity * sizeof(glm::ivec3), nullptr, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, this->posList.size() * sizeof(glm::ivec3), this->posList.data());
      }
    }

    void draw() {
      sprite.instanceDraw(this->posList.size());
    }
  


};
