#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in ivec3 aModel;
layout (location = 3) in int aSelectedTex;

out vec2 fTexCoord;
flat out int fSelectedTex;

uniform ivec3 chunkLoc;

layout (std140) uniform vp {
  mat4 view;
  mat4 projection;
};

void main() {
  mat4 model = mat4(1.0f);
  model[3] = vec4(vec3(aModel + chunkLoc * 32), 1.0f);
  gl_Position = projection * view * model * vec4(aPos, 1.0f);
  fTexCoord = aTexCoords;
  fSelectedTex = aSelectedTex;
}
