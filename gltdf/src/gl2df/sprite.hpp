#pragma once

#include "vertexArray.hpp"
#include "../gltdf/shader.hpp"
#include "texture.hpp"

#include <initializer_list>
#include <vector>

namespace gl2df {

  class Sprite {

    public:
      std::vector<Texture> textures;
      gltdf::Shader shader;
      VertexArray bufferSet;


      Sprite(gltdf::Shader shader, VertexArray bufferSet = VertexArray(), std::initializer_list<Texture> textures = {})
      : shader(shader), bufferSet(bufferSet), textures(textures) {}

      void draw() const {
        this->shader.bind();
        for(const Texture& tex : this->textures) tex.bind();
        this->bufferSet.draw();
      }

      void drawNOBIND() const {
        this->bufferSet.drawNOBIND();
      }

      void bind() const {
        this->shader.bind();
        for(const Texture& tex : this->textures) tex.bind();
        this->bufferSet.bind();
      }

  };

}
