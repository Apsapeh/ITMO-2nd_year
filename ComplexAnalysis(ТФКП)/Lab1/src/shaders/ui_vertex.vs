#version 400 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec4 aCol;

uniform mat4 uProjection;
out vec2 vUV;
out vec4 vCol;

void main() {
    vUV = aUV;
    vCol = aCol;
    gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
}