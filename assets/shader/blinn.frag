#version 450 core

#define PI 3.141592653589793

in vec3 iwposition;
in vec4 ilposition;
in vec2 iuv;
in mat3 ibasis;

// Light definitions
struct PointLight {
    vec3 position;
    vec3 color;
};
uniform PointLight point_lights[10];
uniform int        point_lights_count;

struct DirectionLight {
    vec3 direction;
    vec3 color;
    sampler2DShadow depth_map;
};
uniform DirectionLight direction_light;

// ???
uniform vec3 camera_position;

// Material parameters
uniform float specular_exponent;

uniform sampler2D diffuse_texture;
uniform sampler2D normal_texture;
uniform sampler2D specular_texture;

const float ambient_strength = 0.1f;

out vec4 frag_color;

vec3 ambient(const in vec3 fragment, const in vec3 normal, const in vec3 light_direction, const in vec3 light_color) {
    return ambient_strength * light_color;
}

vec3 diffuse(const in vec3 fragment, const in vec3 normal, const in vec3 light_direction, const in vec3 light_color) {
    float diffuse = clamp(dot(normal, light_direction), 0.0f, 1.0f);
    return diffuse * light_color;
}

vec3 specular(const in vec3 fragment, const in vec3 normal, const in vec3 light_direction, const in vec3 light_color) {
    vec3 view_direction = normalize(camera_position - fragment);
    vec3 half_vector    = normalize(light_direction + view_direction);

    float specular = pow(clamp(dot(normal, half_vector), 0.0f, 1.0f),
        specular_exponent);

    // Multiplying by dot product prevents highlights from showing through objects
    return ((specular_exponent + 8) / (8 * PI)) * specular * light_color * max(dot(normal, light_direction), 0.0f);
}

vec4 point_color(const in vec3 normal, const in PointLight light) {
    vec3 light_delta = light.position - iwposition;
    vec3 light_direction = normalize(light_delta);

    vec3 ambient_color  = ambient(iwposition, normal, light_direction, light.color);
    vec3 diffuse_color  = diffuse(iwposition, normal, light_direction, light.color);
    vec3 specular_color = specular(iwposition, normal, light_direction, light.color);

    float attenuation = 1.0f / pow((length(light_delta) / 15.0f) + 1, 2);

    vec4 ambient_final  = vec4(ambient_color * attenuation, 1.0f);
    vec4 diffuse_final  = vec4(diffuse_color * attenuation, 1.0f);
    vec4 specular_final = vec4(specular_color * attenuation, 1.0f);

    return (ambient_final + diffuse_final + specular_final)
        * texture(diffuse_texture, iuv);
}

vec4 direction_color(const in vec3 normal, const in DirectionLight light) {
    vec3 light_delta = light.direction;
    vec3 light_direction = normalize(light_delta);

    vec3 ambient_color  = ambient(iwposition, normal, light_direction, light.color);
    vec3 diffuse_color  = diffuse(iwposition, normal, light_direction, light.color);
    vec3 specular_color = specular(iwposition, normal, light_direction, light.color);

    // Shadow (using comparison-mode sampling)
    // Light-space fragment position normalized to [0,1]
    vec3 fp = (ilposition.xyz / ilposition.w) * 0.5f + 0.5f;
    vec3 bias = vec3(0, 0, 0.003f);
    float shadow = texture(light.depth_map, fp - bias);

    float attenuation = 1.0f;

    vec4 ambient_final  = vec4(ambient_color * attenuation, 1.0f);
    vec4 diffuse_final  = vec4(diffuse_color * attenuation, 1.0f);
    vec4 specular_final = vec4(specular_color * attenuation, 1.0f);

    return (ambient_final + (diffuse_final + specular_final) * shadow)
        * texture(diffuse_texture, iuv);
}

void main() {
    vec3 sampled_normal = texture(normal_texture, iuv).rgb * 2.0f - 1.0f;
    vec3 normal = normalize(ibasis * sampled_normal);

    for (int i = 0; i < point_lights_count; i++)
        frag_color += point_color(normal, point_lights[i]);

    frag_color += direction_color(normal, direction_light);
}
