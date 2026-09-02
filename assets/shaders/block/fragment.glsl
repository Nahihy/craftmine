#version 330 core

in vec2 fTexCoord;

uniform sampler2D tex[1];

void main() {
  gl_FragColor = texture(tex[0], fTexCoord);
}
