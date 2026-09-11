#include "fastnoiseLite.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <gltdf/gltdf.hpp>
#include <gl2df/gl2df.hpp>
#include <strings.h>
#include <unordered_map>
#include <utility>
#include <vector>

struct IVec2Hash {
  std::size_t operator()(const glm::ivec2& v) const noexcept {
    std::size_t seed = 0;
    seed ^= std::hash<int>{}(v.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= std::hash<int>{}(v.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
};

#define RENDER_DIS 16
#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 128
#define WATER_LEVEL 42

#define BLOCK_FACE_VERTICES {\
  -1.0f,  1.0f, -1.0f,   0.0f, 1.0f,\
  -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,\
   1.0f, -1.0f, -1.0f,   1.0f, 0.0f,\
   1.0f, -1.0f, -1.0f,   1.0f, 0.0f,\
   1.0f,  1.0f, -1.0f,   1.0f, 1.0f,\
  -1.0f,  1.0f, -1.0f,   0.0f, 1.0f,\
}

enum BlockType {
  DIRT, GRASS, WATER, AIR
};

constexpr char* blockTexs[AIR] = {
  TEXTURE_BINARY_DIR"/blocks/dirt.png",
  TEXTURE_BINARY_DIR"/blocks/grass.png",
  TEXTURE_BINARY_DIR"/blocks/water.png"
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



class Chunk {
  public:
    std::array<BlockType, CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH> blockList;
    std::vector<BlockFace> visableFaces;
    bool dirty = true;

    Chunk(const FastNoiseLite& noise, const glm::ivec2& loc) {
      for(int x = 0; x < CHUNK_WIDTH; x++)
        for(int z = 0; z < CHUNK_WIDTH; z++) {
          for(int y = 0; y < WATER_LEVEL; y++)
            this->blockList[indexAt(x, y, z)] = WATER;
          for(int y = WATER_LEVEL; y < CHUNK_HEIGHT; y++)
            this->blockList[indexAt(x, y, z)] = AIR;
        }

      for(int x = 0; x < CHUNK_WIDTH; x++)
        for(int z = 0; z < CHUNK_WIDTH; z++) {
          int height = (noise.GetNoise((float)(x + loc.x * 16), (float)(z + loc.y * 16)) + 1) * 60;
          if(height >= WATER_LEVEL) this->blockList[indexAt(x, height, z)] = GRASS;
          for(int y = 0; y < height; y++)
            this->blockList[indexAt(x, y, z)] = DIRT;
        }
      glGenBuffers(1, &this->VBO);
    }

    ~Chunk() {
      glDeleteBuffers(1, &this->VBO);
    }

    int inline indexAt(int x, int y, int z) const {
      if(x >= CHUNK_WIDTH || y >= CHUNK_HEIGHT || z >= CHUNK_WIDTH || x < 0 || y < 0 || z < 0) return -1;
      return (x + y * CHUNK_WIDTH + z * CHUNK_WIDTH * CHUNK_HEIGHT);
    }

    BlockType inline blockAt(int x, int y, int z) const {
      if(x >= CHUNK_WIDTH || y >= CHUNK_HEIGHT || z >= CHUNK_WIDTH || x < 0 || y < 0 || z < 0) return AIR;
      return this->blockList[indexAt(x, y, z)];
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
      int oldSize = this->visableFaces.size();
      this->visableFaces.clear();
      this->visableFaces.reserve(oldSize);
      for(int x = 0; x < CHUNK_WIDTH; x++)
        for(int y = 0; y < CHUNK_HEIGHT; y++)
          for(int z = 0; z < CHUNK_WIDTH; z++) {
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
    }

};

class World {

  public:
    std::unordered_map<glm::ivec2, Chunk, IVec2Hash> chunks;
    gl2df::VertexArray blockVertices;
    gltdf::Shader blockShader;

    World() : blockVertices(gl2df::VertexArray({BLOCK_FACE_VERTICES}, {}, {{0, 3, 5 * sizeof(float), 0}, 
    {1, 2, 5 * sizeof(float), 3 * sizeof(float)}})), blockShader(gltdf::Shader("block/vertex.glsl", "block/fragment.glsl")) {
      this->noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
      this->noise.SetSeed(rand());
      this->noise.SetFrequency(0.005f);
      char uniBuff[8];
      this->blockVertices.bind();
      glVertexAttribDivisor(2, 1);
      glEnableVertexAttribArray(2);
      glVertexAttribDivisor(3, 1);
      glEnableVertexAttribArray(3);
      glBindVertexArray(0);
      loadTextureArray();
    }

    ~World() {
      glDeleteTextures(1, &this->textures);
    }

    void draw(const glm::ivec2& camLoc) {
      if(this->camLoc != camLoc / 32) {
        this->camLoc = camLoc / 32;
        updateChunks();
      }
      this->blockShader.bind();
      glBindTexture(GL_TEXTURE_2D_ARRAY, this->textures);
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
      glFrontFace(GL_CW);
      for(auto& [loc, chunk] : this->chunks) {
        this->blockShader.setIVec2NOBIND("chunkLoc", loc);
        chunk.draw(this->blockVertices);
      }
      glDisable(GL_CULL_FACE);
    }
 
  private:

    unsigned int textures;
    glm::ivec2 camLoc;
    FastNoiseLite noise;

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

    void updateChunks() {
      this->chunks.clear();
      this->chunks.reserve(((RENDER_DIS * 2) + 1) * ((RENDER_DIS * 2) + 1));
      this->chunks.try_emplace(this->camLoc, this->noise, this->camLoc);
      glm::ivec2 chunkLoc;
      for(int x = 1; x <= RENDER_DIS; x++) {
        chunkLoc = glm::ivec2(this->camLoc.x - x, this->camLoc.y);
        this->chunks.try_emplace(chunkLoc, this->noise, chunkLoc);
        chunkLoc = glm::ivec2(this->camLoc.x + x, this->camLoc.y);
        this->chunks.try_emplace(chunkLoc, this->noise, chunkLoc);
        for(int z = 1; z <= RENDER_DIS; z++) {
          chunkLoc = glm::ivec2(this->camLoc.x - x, this->camLoc.y - z);
          this->chunks.try_emplace(chunkLoc, this->noise, chunkLoc);
          chunkLoc = glm::ivec2(this->camLoc.x + x, this->camLoc.y - z);
          this->chunks.try_emplace(chunkLoc, this->noise, chunkLoc);
          chunkLoc = glm::ivec2(this->camLoc.x - x, this->camLoc.y + z);
          this->chunks.try_emplace(chunkLoc, this->noise, chunkLoc);
          chunkLoc = glm::ivec2(this->camLoc.x + x, this->camLoc.y + z);
          this->chunks.try_emplace(chunkLoc, this->noise, chunkLoc);
        }
      }
      for(int z = 0; z <= RENDER_DIS; z++) {
        chunkLoc = glm::ivec2(this->camLoc.x, this->camLoc.y - z);
        this->chunks.try_emplace(chunkLoc, this->noise, chunkLoc);
        chunkLoc = glm::ivec2(this->camLoc.x, this->camLoc.y + z);
        this->chunks.try_emplace(chunkLoc, this->noise, chunkLoc);
      }
    }

};
