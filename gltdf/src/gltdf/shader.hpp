#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace gltdf {

  class Shader {
    public:
      unsigned int ID;
      Shader(const std::string& vertexPath, const std::string& fragmentPath) {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
          vShaderFile.open(SHADER_BINARY_DIR + vertexPath);
          fShaderFile.open(SHADER_BINARY_DIR + fragmentPath);
          std::stringstream vShaderStream, fShaderStream;
          vShaderStream << vShaderFile.rdbuf();
          fShaderStream << fShaderFile.rdbuf();
          vShaderFile.close();
          fShaderFile.close();
          vertexCode   = vShaderStream.str();
          fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e) {
          std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        unsigned int vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX", vertexPath);
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT", fragmentPath);
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM", "");
        glDeleteShader(vertex);
        glDeleteShader(fragment);
      }

      void bind() const { 
        glUseProgram(ID); 
      }
      void setBoolNOBIND(const std::string &name, bool value) const {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
      }
      void setIntNOBIND(const std::string &name, int value) const { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
      }
      void setFloatNOBIND(const std::string &name, float value) const { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
      }
      void setVec2NOBIND(const std::string &name, const glm::vec2 &value) const { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
      }
      void setVec2NOBIND(const std::string &name, float x, float y) const { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
      }
      void setVec3NOBIND(const std::string &name, const glm::vec3 &value) const { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
      }
      void setVec3NOBIND(const std::string &name, float x, float y, float z) const { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
      }
      void setVec4NOBIND(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
      }
      void setVec4NOBIND(const std::string &name, float x, float y, float z, float w) const { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
      }
      void setMat2NOBIND(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
      }
      void setMat3NOBIND(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
      }
      void setMat4NOBIND(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
      }
      void setBool(const std::string &name, bool value) const {         
        glUseProgram(ID); 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
      }
      void setInt(const std::string &name, int value) const { 
        glUseProgram(ID); 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
      }
      void setFloat(const std::string &name, float value) const { 
        glUseProgram(ID); 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
      }
      void setVec2(const std::string &name, const glm::vec2 &value) const { 
        glUseProgram(ID); 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
      }
      void setVec2(const std::string &name, float x, float y) const { 
        glUseProgram(ID); 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
      }
      void setVec3(const std::string &name, const glm::vec3 &value) const { 
        glUseProgram(ID); 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
      }
      void setVec3(const std::string &name, float x, float y, float z) const { 
        glUseProgram(ID); 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
      }
      void setVec4(const std::string &name, const glm::vec4 &value) const { 
        glUseProgram(ID); 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
      }
      void setVec4(const std::string &name, float x, float y, float z, float w) const { 
        glUseProgram(ID); 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
      }
      void setMat2(const std::string &name, const glm::mat2 &mat) const {
        glUseProgram(ID); 
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
      }
      void setMat3(const std::string &name, const glm::mat3 &mat) const {
        glUseProgram(ID); 
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
      }
      void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUseProgram(ID); 
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
      }
    private:
      void checkCompileErrors(unsigned int shader, std::string type, const std::string& loc) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
          glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
          if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << " in: " << loc << "\n" << infoLog << "\n" << std::endl;
          }
        } else {
          glGetProgramiv(shader, GL_LINK_STATUS, &success);
          if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n" << std::endl;
          }
        }
      }
  };

}
