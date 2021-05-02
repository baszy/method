#version 450 core

/* This shader generates the view of the sky through a quad. The algorithm
 * used is the same as the one presented in:
 *
 * Nishita, T., T. Sirai, K. Tadamura, E. Nakamae. 1993.
 * "Display of the Earth Taking into Account Atmospheric Scattering."
 */

#define PI 3.141592653589793

in vec4 point_coord;
in vec2 tex_coord;

uniform vec3 camera_position;

uniform vec3 sun_direction;
uniform vec3 sun_color;

uniform sampler2D noise;

out vec4 frag_color;

// Ground and atmosphere radii and position
const float r_ground = 6.37e6;
const float r_atmosphere = 6.43e6;

const vec3 p_centre = vec3(0, -r_ground, 0);

const vec3 rayleigh_beta = vec3(3.8e-6, 13.5e-6, 33.1e-6);
const vec3 mie_beta = vec3(21.0e-6, 21.0e-6, 21.0e-6);

const float rayleigh_scale_height = 8.5e3;
const float mie_scale_height = 1.5e3;

const int step_samples = 10;
const int optical_samples = 5;

// returns vec2(enter distance, exit distance)
bool ray_sphere(vec3 ray_origin, vec3 ray_direction,
                vec3 sphere_origin, float sphere_r,
                out float enter, out float exit) {

    vec3 delta = sphere_origin - ray_origin;

    float centre_dist = dot(delta, ray_direction);
    if (length(delta) > sphere_r && centre_dist < 0) return false;

    float r2 = sphere_r * sphere_r;
    float d2 = dot(delta, delta) - centre_dist * centre_dist;
    if (d2 > r2) return false;

    float chord = sqrt(r2 - d2);

    enter = centre_dist - chord;
    exit = centre_dist + chord;
    return true;
}

// Henyey-Greenstein / Cornette phase function as described in Nishita et al. 1993
float phase(float g, float cosa) {
    float cos2a = cosa * cosa;
    float gg = g * g;

    return
          (3.0f * (1.0f - gg)    *                (1 + cos2a))              /
    /*--------------------------   ------------------------------------------*/
       (8.0f * PI * (2.0f + gg)  *  pow(gg - 2 * g * cosa + 1, 3.0f / 2.0f));
}

vec2 get_optical_depth(vec3 ray_origin, vec3 ray_direction) {
    // Distance to edge of atmosphere in the given direction
    float enter; float exit;
    float p_enter; float p_exit;
    if (!ray_sphere(ray_origin, ray_direction, p_centre, r_atmosphere, enter, exit))
        return vec2(0);
    //if (ray_sphere(ray_origin, ray_direction, p_centre, r_ground, p_enter, p_exit))
    //    if (p_enter > 0) exit = min(p_enter, exit);
    enter = max(enter, 0.0f);
    float step = (exit - enter) / float(optical_samples);

    // .x = Rayleigh optical depth, .y = Mie optical depth;
    vec2 optical_depth = vec2(0);

    for (int i = 0; i < optical_samples; i++) {
        float dist = (i + 0.5f) * step;
        vec3 point = ray_origin + (enter + dist) * ray_direction;

        float altitude = distance(point, p_centre) - r_ground;
        //if (altitude < 0) return vec2(1e32);

        vec2 current_depth = vec2(exp(-altitude / rayleigh_scale_height) * step,
                                  exp(-altitude / mie_scale_height) * step);
        optical_depth += current_depth;
    }

    return optical_depth;
}

vec4 sky(vec3 ray_origin, vec3 ray_direction) {
    vec3 rayleigh_color = vec3(0);
    vec3 mie_color = vec3(0);

    // Distance to edge of atmosphere in the given direction
    float enter; float exit;
    float p_enter; float p_exit;
    if (!ray_sphere(ray_origin, ray_direction, p_centre, r_atmosphere, enter, exit))
        return vec4(0);
    //if (ray_sphere(ray_origin, ray_direction, p_centre, r_ground, p_enter, p_exit))
    //    if (p_enter > 0) exit = min(p_enter, exit);
    enter = max(enter, 0.0f);
    float step = (exit - enter) / float(step_samples);

    // .x = Rayleigh optical depth, .y = Mie optical depth;
    vec2 optical_depth = vec2(0);

    for (int i = 0; i < step_samples; i++) {
        float dist = (i + 0.5f) * step;
        vec3 point = ray_origin + (enter + dist) * ray_direction;

        float altitude = distance(point, p_centre) - r_ground;
        // if (altitude < 0) break;

        vec2 current_depth = vec2(exp(-altitude / rayleigh_scale_height) * step,
                                  exp(-altitude / mie_scale_height) * step);
        optical_depth += current_depth;

        // TODO: inner loop is Slow ! ! !
        vec2 light_optical_depth = get_optical_depth(point, sun_direction);

        vec3 attenuation = exp(-rayleigh_beta * (optical_depth.x + light_optical_depth.x)
                               -mie_beta * (optical_depth.y + light_optical_depth.y));

        rayleigh_color += attenuation * current_depth.x;
        mie_color      += attenuation * current_depth.y;
    }

    // These better be normalized...
    float cosa = dot(ray_direction, sun_direction);
    float rayleigh_phase = phase(0.0, cosa);
    float mie_phase = phase(0.99, cosa);

    return vec4(((rayleigh_color * rayleigh_beta * rayleigh_phase)
               + (mie_color * mie_beta * mie_phase)) * 20.0f, 1.0f);
}

vec4 tonemap(vec4 color) {
    return 1.0f - exp(-1.0f * color);
}

void main() {
    // Polar coordinates are no longer needed for cubemap generation
    /* float longitude = point_coord.x * PI;
       float latitude = point_coord.y * PI_2;
       vec3 direction = vec3(cos(latitude) * sin(longitude),
                             sin(latitude),
                             cos(latitude) * cos(longitude)); */

    // TODO: Unflip this
    vec3 direction = vec3(point_coord.x, -point_coord.y, point_coord.z);
    frag_color = tonemap(sky(camera_position, normalize(direction)) * texture(noise, tex_coord));
}
