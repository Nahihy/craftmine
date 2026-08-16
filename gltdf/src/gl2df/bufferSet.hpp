#pragma once

#include <glad/glad.h>
#include <vector>

namespace gl2df {

  struct VertexAttrib {
    unsigned int index;
    unsigned int size;
    unsigned int stride;
    long offset;
  };

  #define DEFAULT_VERTICES_VAL \
    -1.0f, -1.0f,    0.0f, 1.0f,\
     1.0f,  1.0f,    1.0f, 1.0f,\
    -1.0f, -1.0f,    0.0f, 0.0f,\
     1.0f, -1.0f,    1.0f, 0.0f



  static const float DEFAULT_VERTICES[] = {DEFAULT_VERTICES_VAL};

  #define DEFAULT_VERTEX_COUNT 16

  #define DEFAULT_INDICES_VAL \
    0, 1, 2,\
    1, 2, 3

  static unsigned int DEFAULT_INDICES[] = {DEFAULT_INDICES_VAL};

  #define DEFAULT_INDEX_COUNT 6

  #define DEFAULT_VERTEXATTRIB_VAL \
    {0, 2, 4 * sizeof(float), 0},\
    {1, 2, 4 * sizeof(float), 2 * sizeof(float)}

  static VertexAttrib DEFAULT_VERTEXATTRIB[] = {DEFAULT_VERTEXATTRIB_VAL};

  #define DEFAULT_VERTEXATTRIB_COUNT 2


  class BufferSet {

    private:
      unsigned int VAO; 
      unsigned int VBO;
      unsigned int EBO;
      unsigned int elementCount;
      bool usingElementArray;
      std::vector<float> vertices;
      std::vector<unsigned int> indices;
      std::vector<VertexAttrib> attribs;

      void buildBufferSet() {

        glGenVertexArrays(1, &this->VAO);

        glBindVertexArray(this->VAO);

        glGenBuffers(1, &this->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW);

        if(this->indices.empty()) {
          this->usingElementArray = false;
          this->elementCount = this->vertices.size();
        } else {
          glGenBuffers(1, &this->EBO);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->indices.size(), this->indices.data(), GL_STATIC_DRAW);
        }

        for(const VertexAttrib& attrib : this->attribs) {
          glVertexAttribPointer(attrib.index, attrib.size, GL_FLOAT, GL_FALSE, attrib.stride, (void*)attrib.offset);
          glEnableVertexAttribArray(attrib.index);
        }

        glBindVertexArray(0);
      }

    public:
      BufferSet(float* vertices, int vertexCount = DEFAULT_VERTEX_COUNT,
           unsigned int* indices = DEFAULT_INDICES, int indexCount = DEFAULT_INDEX_COUNT,
           VertexAttrib* attribs = DEFAULT_VERTEXATTRIB, int attribCount = DEFAULT_VERTEXATTRIB_COUNT)
          : usingElementArray(true), elementCount(indexCount), vertices(vertices, vertices + (vertexCount)),
            attribs(attribs, attribs + (attribCount)) {
        if(indices != nullptr && indexCount != -1) this->indices = std::vector(indices, indices + (indexCount));
        buildBufferSet();
      }

      BufferSet(const std::vector<float>& vertices = 
                  std::vector(DEFAULT_VERTICES, DEFAULT_VERTICES + (DEFAULT_VERTEX_COUNT * sizeof(float))),
                const std::vector<unsigned int>& indices = 
                  std::vector(DEFAULT_INDICES, DEFAULT_INDICES + (DEFAULT_INDEX_COUNT * sizeof(unsigned int))),
                const std::vector<VertexAttrib>& attribs = 
                  std::vector(DEFAULT_VERTEXATTRIB, DEFAULT_VERTEXATTRIB + (DEFAULT_VERTEXATTRIB_COUNT * sizeof(VertexAttrib))))
          : usingElementArray(true), elementCount(indices.size()), vertices(vertices),
            indices(indices), attribs(attribs) {buildBufferSet();}

      ~BufferSet() {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        if(this->usingElementArray) glDeleteBuffers(1, &this->EBO);
      }

      BufferSet(const BufferSet& other) noexcept
      : BufferSet(other.vertices, other.indices, other.attribs) {} 


      BufferSet& operator=(const BufferSet& other) noexcept {

        if(this == &other) return *this;

        this->vertices = other.vertices;
        this->indices = other.indices;
        this->attribs = other.attribs;
        buildBufferSet();
        return *this;
      }

      BufferSet(BufferSet&& other) noexcept : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), vertices(other.vertices), 
      indices(other.indices), attribs(other.attribs), usingElementArray(other.usingElementArray), elementCount(other.elementCount) {
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
      }

      BufferSet& operator=(BufferSet&& other) noexcept {

        if(this == &other) return *this;

        this->VAO = other.VAO;
        this->VBO = other.VBO;
        this->EBO = other.EBO;
        this->vertices = other.vertices;
        this->indices = other.indices;
        this->attribs = other.attribs;
        this->usingElementArray = other.usingElementArray;
        this->elementCount = other.elementCount;

        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;

        return *this;
      }

      void draw(int stride = 0) const {
        glBindVertexArray(this->VAO);
        drawNOBIND(stride);
      }
      
      void drawNOBIND(int stride = 0) const {
        if(this->usingElementArray) 
          glDrawElements(GL_TRIANGLES, this->elementCount, GL_UNSIGNED_INT, (void*)(stride * sizeof(unsigned int)));
        else glDrawArrays(GL_TRIANGLES, stride, this->elementCount);
      }
      void bind() const {
        glBindVertexArray(this->VAO);
      }

  };

}
