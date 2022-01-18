#version 450 core

layout (location = 0) in vec3 position;

out vec3 point;
out vec2 tex;

void main() {
    point = position;
    tex = position.xy;
    gl_Position = vec4(position, 1.0f);
}
