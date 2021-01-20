#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 corrected_normal;
out vec3 frag_position;
out vec2 texture_coord;
out float fog_amount;

const float fog_distance = 0.005f;
const float fog_power = 2.0f;

void main() {
    corrected_normal = mat3(transpose(inverse(model))) * normal;
    frag_position = vec3(model * vec4(position, 1.0f));
    texture_coord = uv;
    vec4 view_position = view * vec4(frag_position, 1.0f); 
    float distance = length(view_position.xyz);
    fog_amount = clamp(exp(-pow((distance * fog_distance), fog_power)), 0.0f, 1.0f);
    gl_Position = projection * view * model * vec4(position, 1.0f);
}
