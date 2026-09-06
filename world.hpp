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

enum Faceloc {
  NEGATIVE_Z = 0,
  NEGATIVE_Y = 1,
  POSITIVE_Z = 2,
  POSITIVE_Y = 3,
  POSITIVE_X = 4,
  NEGATIVE_X = 6
};

struct BlockFace {
  glm::ivec4 loc;
  BlockType tex;

  BlockFace(BlockType tex, int x, int y, int z, int f) : tex(tex), loc(x, y, z, f) {}

};

constexpr char* blockTexs[AIR] = {
  "assets/textures/dirt.png",
  "assets/textures/grass.png"
};



class Chunk {
  public:
    std::vector<BlockType> blockList;
    std::vector<BlockFace> visableFaces;
    bool dirty = true;

    Chunk() {
      this->blockList.reserve(4096);
      for(int i = 0; i < 16; i++)
        for(int j = 0; j < 16; j++)
          for(int k = 0; k < 16; k++)
            this->blockList.emplace_back(k % 2 == 0 ? DIRT : GRASS);
      this->blockList[locAt(10, 5, 12)] = AIR;
      this->blockList[locAt(15, 5, 12)] = AIR;
      glGenBuffers(1, &this->VBO);
    }

    int inline locAt(int x, int y, int z) const {
      if(x >= 16 || y >= 16 || z >= 16 || x < 0 || y < 0 || z < 0) return -1;
      return (x + y * 16 + z * 16 * 16);
    }

    BlockType inline blockAt(int x, int y, int z) const {
      if(x >= 16 || y >= 16 || z >= 16 || x < 0 || y < 0 || z < 0) return AIR;
      return this->blockList[locAt(x, y, z)];
    }

    void draw(const gl2df::VertexArray& vertArr) {
      if(this->dirty) updateFaces();
      vertArr.bind();
      glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
      glVertexAttribIPointer(2, 4, GL_INT, sizeof(BlockFace), (void*)offsetof(BlockFace, loc));
      glVertexAttribIPointer(3, 1, GL_INT, sizeof(BlockFace), (void*)offsetof(BlockFace, tex));
      vertArr.instanceDrawNOBIND(this->visableFaces.size());
    }

    

  private:

    unsigned int VBO;

    void updateFaces() {
      for(int x = 0; x < 16; x++)
        for(int y = 0; y < 16; y++)
          for(int z = 0; z < 16; z++) {
            if(blockAt(x, y, z) == AIR) continue;
            if(blockAt(x, y, z - 1) == AIR) this->visableFaces.emplace_back(blockAt(x, y, z), x * 2, y * 2, z * 2, NEGATIVE_Z);
            if(blockAt(x, y, z + 1) == AIR) this->visableFaces.emplace_back(blockAt(x, y, z), x * 2, y * 2, z * 2, POSITIVE_Z);
            if(blockAt(x, y - 1, z) == AIR) this->visableFaces.emplace_back(blockAt(x, y, z), x * 2, y * 2, z * 2, NEGATIVE_Y);
            if(blockAt(x, y + 1, z) == AIR) this->visableFaces.emplace_back(blockAt(x, y, z), x * 2, y * 2, z * 2, POSITIVE_Y);
            if(blockAt(x - 1, y, z) == AIR) this->visableFaces.emplace_back(blockAt(x, y, z), x * 2, y * 2, z * 2, NEGATIVE_X);
            if(blockAt(x + 1, y, z) == AIR) this->visableFaces.emplace_back(blockAt(x, y, z), x * 2, y * 2, z * 2, POSITIVE_X);
          }
      glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
      glBufferData(GL_ARRAY_BUFFER, this->visableFaces.size() * sizeof(BlockFace), this->visableFaces.data(), GL_DYNAMIC_DRAW);
      this->dirty = false;
      puts("cleaned");
    }

};

class World {

  public:
    std::unordered_map<glm::ivec3, Chunk, IVec3Hash> chunks;
    gl2df::VertexArray blockVertices;
    gltdf::Shader blockShader;

    World() : blockVertices(gl2df::VertexArray({BLOCK_FACE_VERTICES}, {}, {{0, 3, 5 * sizeof(float), 0}, 
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
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
      glFrontFace(GL_CW);
      for(auto& [loc, chunk] : this->chunks) {
        this->blockShader.setIVec3NOBIND("chunkLoc", loc);
        chunk.draw(this->blockVertices);
      }
      glDisable(GL_CULL_FACE);
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
