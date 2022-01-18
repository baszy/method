#include <algorithm>
#include <cstdlib>
#include <iostream>

#include <SDL2/SDL.h>

#include "Camera.hpp"
#include "Controller.hpp"
#include "Defs.hpp"
#include "Entity.hpp"
#include "Shader.hpp"
#include "Gui.hpp"
#include "Material.hpp"
#include "Math.hpp"
#include "Matrix.hpp"
#include "MappedFile.hpp"
#include "MeshData.hpp"
#include "ObjLoader.hpp"
#include "PngLoader.hpp"
#include "ReadFile.hpp"
#include "Window.hpp"
#include "World.hpp"
#include "RenderSystem.hpp"
#include "Scene.hpp"
#include "Terrain.hpp"
#include "Vector.hpp"

using namespace method;
using namespace std;

int main(int argc, char * argv[]) {
    ///////////////////////////////////////////////////////////////////////////
    // Create the window + renderer
    ///////////////////////////////////////////////////////////////////////////

    Window window("method_example", IVec2(1920, 1080));
    window.set_swap_mode(SwapMode::IMMEDIATE);
    window.set_fullscreen(false);
    window.grab_cursor(true);

    Framebuffer framebuffer(IVec2(1280, 720), FbType::NORMAL);

    Controller input = Controller();

    MeshManager mesh_manager("assets/model/");
    TextureManager tex_manager("assets/image/");

    mesh_manager.reload("sponza.obj");
    mesh_manager.reload("commode.obj");

    tex_manager.reload("dev/diffuse.tga");
    tex_manager.reload("dev/normal.tga");
    tex_manager.reload("dev/specular.tga");
    tex_manager.reload("commode/diffuse.tga");

    RenderSystem renderer = RenderSystem(mesh_manager, tex_manager);
    renderer.print_gl_info();

    IVec2 size = window.get_dimensions();
    Camera camera = Camera(radians(90.0f), (float)size.x / (float)size.y, 0.1f, 1500.0f, Vec3(1.0f, 0.0f, 0.0f));

    ///////////////////////////////////////////////////////////////////////////
    // Create materials and props
    ///////////////////////////////////////////////////////////////////////////

    Shader shader = Shader();
    shader.load("assets/shader/blinn.vert", ShaderType::VERTEX);
    shader.load("assets/shader/blinn.frag", ShaderType::FRAGMENT);
    shader.compile();

    Material ground("dev/diffuse.tga", "dev/normal.tga", "dev/specular.tga");
    ground.specular_exponent = 4;
    ground.shader = &shader;

    Material wood("commode/diffuse.tga", "dev/normal.tga", "dev/specular.tga");
    wood.specular_exponent = 24;
    wood.shader = &shader;

    ///////////////////////////////////////////////////////////////////////////
    // Create the skybox
    ///////////////////////////////////////////////////////////////////////////

    Shader sky_shader = Shader();
    sky_shader.load("assets/shader/atmosphere.vert", ShaderType::VERTEX);
    sky_shader.load("assets/shader/atmosphere.geom", ShaderType::GEOMETRY);
    sky_shader.load("assets/shader/atmosphere.frag", ShaderType::FRAGMENT);
    sky_shader.compile();

    DirectionLight sun(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f));

    Sky sky(sky_shader, sun);
    sky.draw(camera);

    ///////////////////////////////////////////////////////////////////////////
    // Create the scene
    ///////////////////////////////////////////////////////////////////////////

    Scene scene = Scene(camera, sky);

    Prop sponza("sponza.obj", ground);
    sponza.set_scale(10.0f);
    sponza.set_position(Vec3(0.0f, -1.0f, 0.0f));

    Prop dresser("commode.obj", wood);
    dresser.set_scale(10.0f);
    dresser.set_position(Vec3(0.0f, 0.0f, 0.0f));

    scene.props.push_back(&sponza);
    scene.props.push_back(&dresser);

    for (int i = 0; i < scene.props.size(); i++) {
        printf("[Main][Notification/Other] Prop %d:\n", i);
        printf("[Main][Notification/Other]   material: \n");
        printf("[Main][Notification/Other]     diffuse: %s\n", scene.props[i]->material.diffuse_handle.c_str());
        printf("[Main][Notification/Other]     normal: %s\n", scene.props[i]->material.normal_handle.c_str());
        printf("[Main][Notification/Other]     specular: %s\n", scene.props[i]->material.specular_handle.c_str());
        printf("[Main][Notification/Other]   mesh: %s\n",     scene.props[i]->mesh.c_str());
    }

    //scene.add_point_light(PointLight(Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f)));

    ///////////////////////////////////////////////////////////////////////////
    // Set gameplay conditions
    ///////////////////////////////////////////////////////////////////////////

    Vec2 angles = Vec2(0.0f, 0.0f);
    Vec3 pos = Vec3(0.0f, 0.0f, 0.0f);
    float speed = 25.0f; // 25 units per second = 2.5 m/s

    float daytime = 0.3f;

    ///////////////////////////////////////////////////////////////////////////
    // Main loop
    ///////////////////////////////////////////////////////////////////////////

    renderer.debug_ray(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
    renderer.debug_ray(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
    renderer.debug_ray(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f));

    float runtime = 0.0f;
    unsigned long long last_time = SDL_GetPerformanceCounter();

    int fps_i = 0;

    while (input.running) {
        unsigned long long current_time = SDL_GetPerformanceCounter();
        unsigned long long frame_time = current_time - last_time;
        float ms = ((double)frame_time * 1000.0f) / SDL_GetPerformanceFrequency();
        runtime += ms;
        last_time = current_time;

        input.update();

        angles = angles + Vec2(radians(input.mouse.x * 0.05f),
                               radians(-input.mouse.y * 0.05f));
        angles.y = clamp(angles.y, (float)-M_PI_2 + 0.01f, (float)M_PI_2 - 0.01f);

        if (input.mousewheel.x > 0) daytime += 0.01f;
        else if (input.mousewheel.x < 0) daytime -= 0.01f;
        if (input.mousewheel.y > 0) daytime += 0.01f;
        else if (input.mousewheel.y < 0) daytime -= 0.01f;

        sun.direction = Vec3(sin(daytime), cos(daytime), sin(daytime));
        sky.draw(camera);

        // Speed * time = distance :)
        float distance = speed * (ms / 1000.0f);

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

        renderer.draw(&framebuffer, scene);
        window.present(framebuffer);
        window.swap();
    }

    return 0;
}
