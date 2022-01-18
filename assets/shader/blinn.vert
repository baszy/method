#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light;

// Interpolated variables
out vec3 iwposition;
out vec2 iuv;
out vec4 ilposition;
out mat3 ibasis;

void main() {
    // TODO: normal correction for non-uniform scale
    // corrected_normal = mat3(transpose(inverse(model))) * normal;

    // TODO: tangent-space calculation may prevent shader atrocities
    iwposition = vec3(model * vec4(position, 1.0f));
    ilposition = light * model * vec4(position, 1.0f);
    iuv = uv;
    ibasis = mat3(normalize(vec3(model * vec4(tangent, 0.0f))),
                  normalize(vec3(model * vec4(bitangent, 0.0f))),
                  normalize(vec3(model * vec4(normal, 0.0f))));

    // Transformed position
    gl_Position = projection * view * vec4(iwposition, 1.0f);
}
