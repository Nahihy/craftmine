#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in ivec4 aModel;
layout (location = 3) in int aSelectedTex;

out vec2 fTexCoord;
flat out int fSelectedTex;

uniform ivec2 chunkLoc;

layout (std140) uniform vp {
  mat4 view;
  mat4 projection;
};

mat3 rotateX(float angle) {
  float s = sin(angle);
  float c = cos(angle);
  return mat3(
    1.0, 0.0, 0.0,
    0.0, c,   s,
    0.0, -s,  c
  );
}

mat3 rotateY(float angle) {
  float s = sin(angle);
  float c = cos(angle);
  return mat3(
    c,   0.0, -s,
    0.0, 1.0, 0.0,
    s,   0.0, c
  );
}

void main() {
  mat4 model = mat4(1.0f);
  model[3] = vec4(vec3(aModel.xyz + ivec3(chunkLoc.x, 0, chunkLoc.y) * 32), 1.0f);
  vec3 pos;
  if(aModel.w <= 3) {
    pos = aPos * rotateX(radians(90 * aModel.w));
  } else {
    pos = aPos * rotateY(radians(90 * (aModel.w - 3)));
  }
  gl_Position = projection * view * model * vec4((pos), 1.0f);
  fTexCoord = aTexCoords;
  fSelectedTex = aSelectedTex;
}
