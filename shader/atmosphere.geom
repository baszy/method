#version 450 core

layout (triangles, invocations = 1) in;
layout (triangle_strip, max_vertices = 36) out;

in vec3 point[];
in vec2 tex[];

out vec4 point_coord;
out vec2 tex_coord;

// We have all these transformation matrices so we can send just one quad
// to the GPU and then transform it to provide all six cubemap views
const mat4 matrices[6] = {
    // Positive X transform
    mat4(vec4(0, 0, -1, 0),
         vec4(0, 1, 0, 0),
         vec4(1, 0, 0, 0),
         vec4(0, 0, 0, 1)),
    // Negative X transform
    mat4(vec4(0, 0, 1, 0),
         vec4(0, 1, 0, 0),
         vec4(-1, 0, 0, 0),
         vec4(0, 0, 0, 1)),
    // Positive Y transform
    mat4(vec4(1, 0, 0, 0),
         vec4(0, 0, 1, 0),
         vec4(0, -1, 0, 0),
         vec4(0, 0, 0, 1)),
    // Negative Y transform
    mat4(vec4(1, 0, 0, 0),
         vec4(0, 0, -1, 0),
         vec4(0, 1, 0, 0),
         vec4(0, 0, 0, 1)),
    // Positive Z transform
    mat4(vec4(1, 0, 0, 0),
         vec4(0, 1, 0, 0),
         vec4(0, 0, 1, 0),
         vec4(0, 0, 0, 1)),
    // Negative Z transform
    mat4(vec4(-1, 0, 0, 0),
         vec4(0, 1, 0, 0),
         vec4(0, 0, -1, 0),
         vec4(0, 0, 0, 1))
};

void main() {
    for (int layer = 0; layer < 6; layer++) {
        for (int i = 0; i < gl_in.length(); i++) {
            point_coord = matrices[layer] * vec4(point[i], 1.0f);
            tex_coord = tex[i];

            gl_Layer = layer;
            gl_Position = vec4(point[i], 1.0f);
            EmitVertex();
        }

        EndPrimitive();
    }

}
