#version 330 core

in vec3 corrected_normal;
in vec3 frag_position;
in vec2 texture_coord;
in float fog_amount;

uniform vec3 camera_position;
uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 object_color;
uniform float specular_exponent;

uniform sampler2D diffuse_texture;
uniform sampler2D normal_texture;
uniform sampler2D specular_texture;

out vec4 frag_color;

vec4 sky_color = vec4(0.7f, 0.8f, 0.95f, 1.0f);

void main() {
    float ambient_strength = 0.4f;
    vec3 sampled_normal = texture(normal_texture, texture_coord).xyz; 
    vec3 normal = normalize(sampled_normal * 2.0f - 1.0f);

    // Ambient
    vec3 ambient_color = ambient_strength * light_color;

    // Diffuse
    vec3 light_direction = normalize(light_position - frag_position);
    float diffuse = max(dot(normal, light_direction), 0.0f);
    vec3 diffuse_color = diffuse * light_color;

    // Specular
    vec3 view_direction = normalize(camera_position - frag_position);
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular = pow(max(dot(view_direction, reflect_direction), 0.0f),
        specular_exponent);
    vec3 specular_color = specular * light_color;
    
    // Attenuation
    float distance = length(frag_position - light_position);
    // float attenuation = 1.0f /
    //     (1.0f + 0.007 * distance + 0.0002 * distance * distance);
    float attenuation = 1.0f / pow((distance / 100.0f) + 1, 2); 

    vec3 ambient_attenuated = ambient_color * attenuation;
    vec3 diffuse_attenuated = diffuse_color * attenuation;
    vec3 specular_attenuated = specular_color * attenuation;

    // Final coloration
    // TODO: should we use ambient occlusion texture
    vec4 ambient_final = vec4(ambient_attenuated, 1.0f)
        * texture(diffuse_texture, texture_coord);
    vec4 diffuse_final = vec4(diffuse_attenuated, 1.0f)
        * texture(diffuse_texture, texture_coord);
    vec4 specular_final = vec4(specular_attenuated, 1.0f)
        * texture(specular_texture, texture_coord);

    frag_color = ambient_final + diffuse_final + specular_final;
    frag_color = mix(sky_color, frag_color, fog_amount);
}
