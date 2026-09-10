#include <gltdf/gltdf.hpp>
#include <gl2df/gl2df.hpp>

constexpr char* skyBoxTex[6] = {
  TEXTURE_BINARY_DIR"/skybox/right.jpg",
  TEXTURE_BINARY_DIR"/skybox/left.jpg",
  TEXTURE_BINARY_DIR"/skybox/top.jpg",
  TEXTURE_BINARY_DIR"/skybox/bottom.jpg",
  TEXTURE_BINARY_DIR"/skybox/front.jpg",
  TEXTURE_BINARY_DIR"/skybox/back.jpg"
};

#define SKYBOX_VERTICES\
  -1.0f,  1.0f, -1.0f,\
  -1.0f, -1.0f, -1.0f,\
   1.0f, -1.0f, -1.0f,\
   1.0f, -1.0f, -1.0f,\
   1.0f,  1.0f, -1.0f,\
  -1.0f,  1.0f, -1.0f,\
\
  -1.0f, -1.0f,  1.0f,\
  -1.0f, -1.0f, -1.0f,\
  -1.0f,  1.0f, -1.0f,\
  -1.0f,  1.0f, -1.0f,\
  -1.0f,  1.0f,  1.0f,\
  -1.0f, -1.0f,  1.0f,\
\
   1.0f, -1.0f, -1.0f,\
   1.0f, -1.0f,  1.0f,\
   1.0f,  1.0f,  1.0f,\
   1.0f,  1.0f,  1.0f,\
   1.0f,  1.0f, -1.0f,\
   1.0f, -1.0f, -1.0f,\
\
  -1.0f, -1.0f,  1.0f,\
  -1.0f,  1.0f,  1.0f,\
   1.0f,  1.0f,  1.0f,\
   1.0f,  1.0f,  1.0f,\
   1.0f, -1.0f,  1.0f,\
  -1.0f, -1.0f,  1.0f,\
\
  -1.0f,  1.0f, -1.0f,\
   1.0f,  1.0f, -1.0f,\
   1.0f,  1.0f,  1.0f,\
   1.0f,  1.0f,  1.0f,\
  -1.0f,  1.0f,  1.0f,\
  -1.0f,  1.0f, -1.0f,\
\
  -1.0f, -1.0f, -1.0f,\
  -1.0f, -1.0f,  1.0f,\
   1.0f, -1.0f, -1.0f,\
   1.0f, -1.0f, -1.0f,\
  -1.0f, -1.0f,  1.0f,\
   1.0f, -1.0f,  1.0f\


class SkyBox {

  public:
    gl2df::VertexArray vertArr;
    gltdf::Shader shader;

    SkyBox() : shader("skybox/vertex.glsl", "skybox/fragment.glsl"),
    vertArr({SKYBOX_VERTICES}, {}, {{0, 3, 3 * sizeof(float), 0}}) {loadSkyboxCubeTex();}

    void draw() const {
      glDepthFunc(GL_LEQUAL);
      glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubeTex);
      glDepthMask(GL_FALSE);
      this->shader.bind();
      this->vertArr.draw();
      glDepthMask(GL_TRUE);
    }

  private:
  
    unsigned int cubeTex;

    void loadSkyboxCubeTex() {
      glGenTextures(1, &this->cubeTex);
      glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubeTex);
      int width, height, nrChannels;
      stbi_set_flip_vertically_on_load(false);
      for (unsigned int i = 0; i < 6; i++) {
        unsigned char *data = stbi_load(skyBoxTex[i], &width, &height, &nrChannels, 0);
        if (data) {
          GLenum format = GL_RGB;
          if(nrChannels == 1) format = GL_RED;
          else if(nrChannels == 2) format = GL_RG;
          else if(nrChannels == 3) format = GL_RGB;
          else if(nrChannels == 4) format = GL_RGBA;
          glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                       0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
          );
          stbi_image_free(data);
        } else {
          std::cout << "SkyBox Cubemap tex failed to load at path: " << skyBoxTex[i] << std::endl;
          stbi_image_free(data);
        }
      }
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }


};
