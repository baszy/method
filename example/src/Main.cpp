#include <iostream>

#include <SDL2/SDL.h>

#include "Angle.hpp"
#include "Camera.hpp"
#include "Controller.hpp"
#include "Defs.hpp"
#include "Entity.hpp"
#include "GLShader.hpp"
#include "Material.hpp"
#include "Matrix.hpp"
#include "MeshData.hpp"
#include "OBJLoader.hpp"
#include "ReadFile.hpp"
#include "Window.hpp"
#include "RenderSystem.hpp"
#include "Scene.hpp"
#include "Terrain.hpp"
#include "Vector.hpp"

using namespace method;

float clamp(float input, float min, float max) {
    if (input < min) return min;
    else if (input > max) return max;
    return input;
}

int main() {
    // TODO: This doesnt work with sizes different than the framebuffer ???
    Window window("method_example", IVec2(1920, 1080));
    window.set_swap_mode(SwapMode::VSYNC);
    window.set_fullscreen(true);
    window.grab_cursor(true);

    Framebuffer framebuffer(IVec2(1920, 1080), false);

    MeshManager mesh_manager = MeshManager("example/resource/");
    TextureManager tex_manager = TextureManager("example/img/");

    Controller input = Controller();

    GLShader shader = GLShader();
    shader.load("shader/blinn.vert", ShaderType::VERTEX);
    shader.load("shader/blinn.frag", ShaderType::FRAGMENT);
    shader.compile();

    RenderSystem renderer = RenderSystem(framebuffer, mesh_manager, tex_manager);

    IVec2 size = window.get_dimensions();
    Camera camera = Camera(radians(70.0f), (float)size.x / (float)size.y, 0.1f, 500.0f);

    Material moss_brick = Material();
    moss_brick.diffuse_color = Vec3(0.571f, 0.580f, 0.504f);
    moss_brick.specular_exponent = 4;
    moss_brick.diffuse_map_handle = tex_manager.index_of("moss_brick/diffuse.tga");
    moss_brick.normal_map_handle = tex_manager.index_of("moss_brick/normal.tga");
    moss_brick.displacement_map_handle = tex_manager.index_of("moss_brick/displacement.tga");
    moss_brick.shader = &shader;

    Material wood_floor = Material();
    wood_floor.diffuse_color = Vec3(0.716f, 0.532f, 0.333f);
    wood_floor.specular_exponent = 2048;
    wood_floor.diffuse_map_handle = tex_manager.index_of("wood_floor/diffuse.tga");
    wood_floor.normal_map_handle = tex_manager.index_of("wood_floor/normal.tga");
    wood_floor.displacement_map_handle = tex_manager.index_of("wood_floor/displacement.tga");
    wood_floor.shader = &shader;

    Scene scene = Scene();

    scene.camera = &camera;

    /* You can add point lights to the scene like this:
     *
     * PointLight light1 = { .position = Vec3(0.0f, 0.0f, 0.0f),
     *                       .color = Vec3(1.0f, 0.0f, 0.0f) };
     * PointLight light2 = { .position = Vec3(0.0f, 0.0f, 0.0f),
     *                       .color = Vec3(0.0f, 1.0f, 0.0f) };
     * PointLight light3 = { .position = Vec3(0.0f, 0.0f, 0.0f),
     *                       .color = Vec3(0.0f, 0.0f, 2.2f) };
     * PointLight light4 = { .position = Vec3(0.0f, 0.0f, 0.0f),
     *                       .color = normalize(Vec3(1.0f, 1.0f, 0.0f)) };
     * scene.add_point_light(light1);
     * scene.add_point_light(light2);
     * scene.add_point_light(light3);
     * scene.add_point_light(light4);
     */

    DirectionLight sun = { .direction = Vec3(1.0f, 0.15f, 0.0f),
                           .color = Vec3(1.0f, 1.0f, 1.0f) };
    scene.set_sun(sun);

    Prop plane = Prop(mesh_manager.index_of("plane.obj"), moss_brick);
    scene.props.push_back(&plane);

    // 2 units per second
    float speed = 2.0f;

    // Initial conditions
    Vec2 angles = Vec2(0.0f, 0.0f);
    Vec3 pos = Vec3(0.0f, 0.0f, 0.0f);

    float runtime = 0.0f;
    unsigned int last_time = SDL_GetTicks();
    while (input.running) {
        unsigned int current_time = SDL_GetTicks();
        unsigned int frame_time = current_time - last_time;
        float seconds = (float)frame_time / 1000.0f;
        runtime += seconds;
        last_time = current_time;

        input.update();

        angles = angles + Vec2(radians(input.mouse.x * 0.05f),
                               radians(-input.mouse.y * 0.05f));
        angles.y = clamp(angles.y, -M_PI_2 + 0.01, M_PI_2 - 0.01);

        // Speed * time = distance :)
        float distance = speed * seconds;

        /*
         * scene.point_lights_positions[0] =
         *     Vec3( 4.0f + sin(1 * M_PI_4 + runtime), 1.5f,  4.0f + cos(1 * M_PI_4 + runtime));
         * scene.point_lights_positions[1] =
         *     Vec3(-4.0f + sin(    M_PI_2 + runtime), 1.5f,  4.0f + cos(    M_PI_2 + runtime));
         * scene.point_lights_positions[2] =
         *     Vec3( 4.0f + sin(3 * M_PI_4 + runtime), 1.5f, -4.0f + cos(3 * M_PI_4 + runtime));
         * scene.point_lights_positions[3] =
         *     Vec3(-4.0f + sin(    M_PI   + runtime), 1.5f, -4.0f + cos(    M_PI   + runtime));
         */

        scene.sun.direction = Vec3(sin(runtime / 240.0f), sin(runtime / 240.0f), cos(runtime / 240.0f));

        Vec3 front(cos(angles.x), 0, sin(angles.x));
        Vec3 look(front.x * cos(angles.y),
                  sin(angles.y),
                  front.z * cos(angles.y));
        Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
        Vec3 side = normalize(cross(front, up));

        if (input.direction_1.x > 0) pos = pos + distance * side;
        else if (input.direction_1.x < 0) pos = pos - distance * side;
        if (input.direction_1.y > 0) pos = pos + distance * up;
        else if (input.direction_1.y < 0) pos = pos - distance * up;
        if (input.direction_1.z > 0) pos = pos - distance * front;
        else if (input.direction_1.z < 0) pos = pos + distance * front;

        camera.set_position(pos);
        camera.set_look(pos + look);

        renderer.draw(scene);
        window.draw_framebuffer(framebuffer);
        window.swap();
    }

    return 0;
}
