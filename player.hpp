#pragma once

#include <gltdf/gltdf.hpp>
#include <gl2df/gl2df.hpp>
#include <gl3df/gl3df.hpp>

class Player {
  private:
    unsigned int skyboxUBO, generalUBO;


  public:

    Player() {
      glGenBuffers(1, &this->generalUBO);
      glBindBuffer(GL_UNIFORM_BUFFER, this->generalUBO);
      glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
      glGenBuffers(1, &this->skyboxUBO);
      glBindBuffer(GL_UNIFORM_BUFFER, this->skyboxUBO);
      glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    }

    void updateUBO(glm::mat4& view, const glm::mat4& projection) {
      glBindBuffer(GL_UNIFORM_BUFFER, this->generalUBO);
      glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(view), &view); 
      glBufferSubData(GL_UNIFORM_BUFFER, sizeof(view), sizeof(projection), &projection); 
      glBindBuffer(GL_UNIFORM_BUFFER, 0);

      view = glm::mat4(glm::mat3(view));
      glBindBuffer(GL_UNIFORM_BUFFER, this->skyboxUBO);
      glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(view), &view); 
      glBufferSubData(GL_UNIFORM_BUFFER, sizeof(view), sizeof(projection), &projection); 
      glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void bindToGeneralUBO(const gltdf::Shader& shader, const char* name) const {
      glBindBuffer(GL_UNIFORM_BUFFER, this->generalUBO);
      glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
      glUniformBlockBinding(shader.ID, glGetUniformBlockIndex(shader.ID, name), 1);
      glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->generalUBO);
      glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void bindToskyboxUBO(const gltdf::Shader& shader, const char* name) const {
      glBindBuffer(GL_UNIFORM_BUFFER, this->skyboxUBO);
      glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
      glUniformBlockBinding(shader.ID, glGetUniformBlockIndex(shader.ID, name), 2);
      glBindBufferBase(GL_UNIFORM_BUFFER, 2, this->skyboxUBO);
      glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

};
