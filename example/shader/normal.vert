#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 uv;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertex_color;

void main() {
    vec3 view_normal = mat3(transpose(inverse(view * model))) * normal;
    vertex_color = vec4(normalize(normal) * 0.5f + 0.5f, 1.0f);
    gl_Position = projection * view * model * vec4(position, 1.0f);
}