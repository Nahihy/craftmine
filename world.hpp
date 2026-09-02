#include "glm/ext/matrix_transform.hpp"
#include <cstdint>
#include <cstdio>
#include <gltdf/gltdf.hpp>
#include <gl2df/gl2df.hpp>
#include <unordered_map>
#include <utility>
#include <vector>

struct IVec3Hash {
  std::size_t operator()(const glm::ivec3& v) const noexcept {
    std::size_t seed = 0;
    seed ^= std::hash<int>{}(v.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= std::hash<int>{}(v.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= std::hash<int>{}(v.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
};

#define BLOCK_FACE_VERTICES {\
  -1.0f,  1.0f, -1.0f,   0.0f, 1.0f,\
  -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,\
   1.0f, -1.0f, -1.0f,   1.0f, 0.0f,\
   1.0f, -1.0f, -1.0f,   1.0f, 0.0f,\
   1.0f,  1.0f, -1.0f,   1.0f, 1.0f,\
  -1.0f,  1.0f, -1.0f,   0.0f, 1.0f,\
}

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


enum BlockType {
  DIRT, AIR
};

constexpr char* blockTexs[AIR] = {
  "grass/dirt.png"
};


class Chunk {
  public:
    std::vector<BlockType> blockList;
    std::vector<glm::ivec3> blockVertices;
    
    unsigned int VBO;

    Chunk() {
      this->blockList.reserve(4096);
      for(int i = 0; i < 4096; i++) this->blockList.emplace_back(DIRT);
      this->blockVertices.reserve(4096);
      for(int i = 0; i < 16; i++)
        for(int j = 0; j < 16; j++)
          for(int k = 0; k < 16; k++)
            this->blockVertices.emplace_back(i * 2, j * 2, k * 2);
      glGenBuffers(1, &this->VBO);
      glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
      glBufferData(GL_ARRAY_BUFFER, 4096 * sizeof(glm::ivec3), this->blockVertices.data(), GL_DYNAMIC_DRAW);
    }


    void draw(const gl2df::VertexArray& vertArr) const {
      vertArr.bind();
      glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
      glVertexAttribIPointer(2, 3, GL_INT, sizeof(glm::ivec3), (void*)0);
      vertArr.instanceDrawNOBIND(this->blockVertices.size());
    }

};

class World {

  public:
    std::unordered_map<glm::ivec3, Chunk, IVec3Hash> chunks;
    gl2df::VertexArray blockVertices;
    gltdf::Shader blockShader;
    std::vector<gl2df::Texture> textures; 


    World() : blockVertices(gl2df::VertexArray({BLOCK_VERTICES}, {}, {{0, 3, 5 * sizeof(float), 0}, 
    {1, 2, 5 * sizeof(float), 3 * sizeof(float)}})), blockShader(gltdf::Shader("block/vertex.glsl", "block/fragment.glsl")) {
      for(int i = 0; i < AIR; i++) this->textures.emplace_back(blockTexs[i], GL_TEXTURE0 + i);
      this->blockShader.bind();
      char uniBuff[8];
      for(int i = 0; i < AIR; i++) {
        snprintf(uniBuff, 8, "tex[%d]", i);
        this->blockShader.setIntNOBIND(uniBuff, i);
      }
      this->blockVertices.bind();
      glVertexAttribDivisor(2, 1);
      glEnableVertexAttribArray(2);
      glBindVertexArray(0);
    }



    void draw() {
      this->blockShader.bind();
      for(const gl2df::Texture& t : this->textures) t.bind();
      for(const auto& [loc, chunk] : this->chunks) {
        this->blockShader.setIVec3NOBIND("chunkLoc", loc);
        chunk.draw(this->blockVertices);
      }
    }
  

};
