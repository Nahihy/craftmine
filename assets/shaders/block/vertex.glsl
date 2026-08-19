#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 fTexCoord;

uniform mat4 models[1024];

layout (std140) uniform vp {
  mat4 view;
  mat4 projection;
};

void main() {
  gl_Position = projection * view * models[gl_InstanceID] * vec4(aPos, 1.0f);
  fTexCoord = aTexCoords;
}
