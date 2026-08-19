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
      VertexArray vertArr;


      Sprite(gltdf::Shader shader, VertexArray vertArr = VertexArray(), std::initializer_list<Texture> textures = {})
      : shader(shader), vertArr(vertArr), textures(textures) {}

      void draw() const {
        this->shader.bind();
        for(const Texture& tex : this->textures) tex.bind();
        this->vertArr.draw();
      }

      void drawNOBIND() const {
        this->vertArr.drawNOBIND();
      }

      void instanceDraw(int amount) const {
        this->shader.bind();
        for(const Texture& tex : this->textures) tex.bind();
        this->vertArr.instanceDraw(amount);
      }

      void instanceDrawNOBIND(int amount) const {
        this->vertArr.instanceDrawNOBIND(amount);
      }

      void bind() const {
        this->shader.bind();
        for(const Texture& tex : this->textures) tex.bind();
        this->vertArr.bind();
      }

  };

}
