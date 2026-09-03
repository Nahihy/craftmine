#version 330 core

in vec2 fTexCoord;
flat in int fSelectedTex;

uniform sampler2DArray tex;

void main() {
  gl_FragColor = texture(tex, vec3(fTexCoord, fSelectedTex));
}
