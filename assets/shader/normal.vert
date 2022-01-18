#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 uv;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertex_color;

void main() {
    vec3 view_normal = mat3(transpose(inverse(view * model))) * normal;
    //vertex_color = vec4(normalize(cross(tangent,bitangent)) * 0.5f + 0.5f, 1.0f);
    vertex_color = vec4(normal * 0.5f + 0.5f, 1.0f);
    gl_Position = projection * view * model * vec4(position, 1.0f);
}
