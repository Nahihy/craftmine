#pragma once

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <stb_image.h>

#include "../gltdf/shader.hpp"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace gl3df {

  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
  };

  struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
  };  

  class Mesh {
    public:
      std::vector<Vertex> vertices;
      std::vector<unsigned int> indices;
      std::vector<Texture> textures;

      Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
      : vertices(vertices), indices(indices), textures(textures) {setupMesh();}

      void draw(gltdf::Shader &shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        shader.bind();
        for(unsigned int i = 0; i < textures.size(); i++) {
          glActiveTexture(GL_TEXTURE0 + i);
          std::string number;
          std::string name = textures[i].type;
          if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
          else if(name == "texture_specular")
            number = std::to_string(specularNr++);

          shader.setInt(("material." + name + number).c_str(), i);
          glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
      } 

      unsigned int VAO, VBO, EBO;
    private:

      void setupMesh() {

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                     &indices[0], GL_STATIC_DRAW);


        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        glBindVertexArray(0);
      }  

  }; 

  unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
      GLenum format = GL_RGB;
      if (nrComponents == 1) format = GL_RED;
      else if (nrComponents == 2) format = GL_RG;
      else if (nrComponents == 3) format = GL_RGB;
      else if (nrComponents == 4) format = GL_RGBA;

      glBindTexture(GL_TEXTURE_2D, textureID);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);
    } else {
      std::cout << "Texture failed to load at path: " << path << std::endl;
      stbi_image_free(data);
    }

    return textureID;
  }

}
