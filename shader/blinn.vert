#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float fog_amount;
out vec3 frag_position;
out vec2 texture_coord;
out mat3 basis_matrix;

const float fog_distance = 0.001f;
const float fog_power = 2.0f;

void main() {
    // corrected_normal = mat3(transpose(inverse(model))) * normal;
    frag_position = vec3(model * vec4(position, 1.0f));
    texture_coord = uv;

    // Fog calculation
    vec4 view_position = view * vec4(frag_position, 1.0f);
    float distance = length(view_position.xyz);
    fog_amount = clamp(exp(-pow((distance * fog_distance), fog_power)), 0.0f, 1.0f);

    // TODO: we should switch to tangent-space calculations to avoid this and
    // other fragment shader atrocities
    basis_matrix = mat3(normalize(vec3(model * vec4(tangent, 0.0f))),
                        normalize(vec3(model * vec4(bitangent, 0.0f))),
                        normalize(vec3(model * vec4(normal, 0.0f))));

    // Transformed position
    gl_Position = projection * view * model * vec4(position, 1.0f);
}
