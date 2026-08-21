#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aModel;

out vec2 fTexCoord;

uniform mat4 models[1024];

layout (std140) uniform vp {
  mat4 view;
  mat4 projection;
};

void main() {
  mat4 model = mat4(1.0f);
  model[3] = vec4(aModel.xyz, 1.0f);
  gl_Position = projection * view * model * vec4(aPos, 1.0f);
  fTexCoord = aTexCoords;
}
