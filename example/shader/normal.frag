#version 330 core

in vec4 vertex_color;

void main() {
    gl_FragColor = vertex_color;
}