#include <glad/glad.h>

#include <iostream>

namespace gltdf {

  class Framebuffer {

    public:

      unsigned int fbo, cbo, rbo;  
      unsigned int width, height;

      Framebuffer(unsigned int width, unsigned int height) : width(width), height(height) {
        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);   

        glGenTextures(1, &this->cbo);
        glBindTexture(GL_TEXTURE_2D, this->cbo);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->cbo, 0);  

        glGenRenderbuffers(1, &this->rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, this->rbo); 
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);  
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
          std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
        glBindFramebuffer(GL_FRAMEBUFFER, 0); 

      }

      ~Framebuffer() {
        glDeleteFramebuffers(1, &this->fbo);
        glDeleteTextures(1, &this->cbo);
        glDeleteRenderbuffers(1, &this->rbo);
      }

      void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
      }

      void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
      }

      void bindTex() {
        glBindTexture(GL_TEXTURE_2D, this->cbo);
      }

      void resize(unsigned int width, unsigned int height) {
        this->width = width;
        this->height = height;

        glBindTexture(GL_TEXTURE_2D, this->cbo);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->width, this->height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
      }




  };

}
