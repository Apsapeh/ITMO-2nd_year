#version 400 core

in vec2 vUV;
in vec4 vCol;

uniform sampler2D uTexture;
out vec4 FragColor;

void main() {
    FragColor = vCol * texture(uTexture, vUV);
}