#include "glm/ext/matrix_transform.hpp"
#include <cstddef>
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
  DIRT, GRASS, AIR
};

struct Block {
  glm::ivec3 loc;
  BlockType type;

  Block(BlockType type, int x, int y, int z) : type(type), loc(x, y, z) {}

};

constexpr char* blockTexs[AIR] = {
  "assets/textures/dirt.png",
  "assets/textures/grass.png"
};



class Chunk {
  public:
    std::vector<Block> blockList;
    // std::vector<glm::ivec3> blockVertices;

    Chunk() {
      this->blockList.reserve(4096);
      for(int i = 0; i < 16; i++)
        for(int j = 0; j < 16; j++)
          for(int k = 0; k < 16; k++)
            this->blockList.emplace_back(k % 2 == 0 ? DIRT : GRASS, i * 2, j * 2, k * 2);
      glGenBuffers(1, &this->VBO);
      glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
      glBufferData(GL_ARRAY_BUFFER, 4096 * sizeof(Block), this->blockList.data(), GL_DYNAMIC_DRAW);
    }


    void draw(const gl2df::VertexArray& vertArr) const {
      vertArr.bind();
      glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
      glVertexAttribIPointer(2, 3, GL_INT, sizeof(Block), (void*)0);
      glVertexAttribIPointer(3, 1, GL_INT, sizeof(Block), (void*)offsetof(Block, type));
      vertArr.instanceDrawNOBIND(this->blockList.size());
    }

  private:

    unsigned int VBO;

};

class World {

  public:
    std::unordered_map<glm::ivec3, Chunk, IVec3Hash> chunks;
    gl2df::VertexArray blockVertices;
    gltdf::Shader blockShader;

    World() : blockVertices(gl2df::VertexArray({BLOCK_VERTICES}, {}, {{0, 3, 5 * sizeof(float), 0}, 
    {1, 2, 5 * sizeof(float), 3 * sizeof(float)}})), blockShader(gltdf::Shader("block/vertex.glsl", "block/fragment.glsl")) {
      this->blockShader.bind();
      char uniBuff[8];
      this->blockVertices.bind();
      glVertexAttribDivisor(2, 1);
      glEnableVertexAttribArray(2);
      glVertexAttribDivisor(3, 1);
      glEnableVertexAttribArray(3);
      glBindVertexArray(0);
      loadTextureArray();
    }



    void draw() {
      this->blockShader.bind();
      glBindTexture(GL_TEXTURE_2D_ARRAY, this->textures);
      for(const auto& [loc, chunk] : this->chunks) {
        this->blockShader.setIVec3NOBIND("chunkLoc", loc);
        chunk.draw(this->blockVertices);
      }
    }
 
  private:

    unsigned int textures;

    void loadTextureArray() {
      glGenTextures(1, &this->textures);
      glBindTexture(GL_TEXTURE_2D_ARRAY, this->textures);

      int width, height, channels;
      unsigned char* data = stbi_load(blockTexs[0], &width, &height, &channels, 0);
      if(!data) return;

      GLenum internalFormat = (channels == 4) ? GL_RGBA8 : GL_RGB8;
      GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
      glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, width, height, AIR, 0, format, GL_UNSIGNED_BYTE, nullptr);
      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, 1, format, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);

      for(int i = 1; i < AIR; i++) {
        data = stbi_load(blockTexs[i], &width, &height, &channels, 0);
        if (data) {
          glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, format, GL_UNSIGNED_BYTE, data);
          stbi_image_free(data);
        }
      }

      glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

};
