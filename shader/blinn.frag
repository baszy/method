#version 450 core

#define PI 3.141592653589793

in float fog_amount;
in vec3 frag_position;
in vec2 texture_coord;
in mat3 basis_matrix;

uniform vec3 camera_position;

uniform int point_lights_length;
uniform vec3 point_lights_positions[10];
uniform vec3 point_lights_colors[10];

uniform vec3 sun_direction;
uniform vec3 sun_color;

uniform vec3 object_color;
uniform float specular_exponent;

uniform sampler2D diffuse_texture;
uniform sampler2D normal_texture;
uniform sampler2D displacement_texture;

out vec4 frag_color;

float ambient_strength = 0.1f;
vec4 sky_color = vec4(0.637f, 0.793f, 1.5f, 1.0f);

vec4 color(vec3 point_normal, vec3 light_delta, vec3 light_color) {
    vec3 light_direction = normalize(light_delta);

    // Ambient
    vec3 ambient_color = ambient_strength * light_color;

    // Diffuse
    float diffuse = clamp(dot(point_normal, light_direction), 0.0f, 1.0f);
    vec3 diffuse_color = (diffuse * light_color);

    // Specular
    vec3 view_direction = normalize(camera_position - frag_position);
    vec3 half_vector = normalize(light_direction + view_direction);
    float specular = pow(clamp(dot(point_normal, half_vector), 0.0f, 1.0f),
        specular_exponent);

    // We cut off specular highlights if lit from behind, otherwise the
    // specular highlight will appear through the object
    // vec3 specular_color = vec3(0.0f);
    // if (dot(point_normal, light_direction) > 0)
    //     specular_color = ((specular_exponent + 8) / (8 * PI))
    //                    * specular * light_color;

    // Alternatively, we can do something like this, but it would make specular
    // highlights much less instense at the edges
    vec3 specular_color = ((specular_exponent + 8) / (8 * PI)) * specular
        * light_color * max(dot(point_normal, light_direction), 0.0f);

    // Attenuation (for directional lights its different)
    // float attenuation = 1.0f / pow((length(light_delta) / 15.0f) + 1, 2);
    float attenuation = 1.0f;

    // Final color is known
    vec4 ambient_final = vec4(ambient_color * attenuation, 1.0f);
    vec4 diffuse_final = vec4(diffuse_color * attenuation, 1.0f);
    vec4 specular_final = vec4(specular_color * attenuation, 1.0f);

    return (ambient_final + diffuse_final + specular_final)
           * texture(diffuse_texture, texture_coord);
}

void main() {
    vec3 sampled_normal = texture(normal_texture, texture_coord).rgb;
    vec3 normal = normalize(basis_matrix * (sampled_normal * 2.0f - 1.0f));

    for (int i = 0; i < point_lights_length; i++)
        frag_color += color(normal, point_lights_positions[i] - frag_position,
                            point_lights_colors[i]);
    frag_color += color(normal, sun_direction, sun_color);

    frag_color = mix(sky_color, frag_color, fog_amount);
}
