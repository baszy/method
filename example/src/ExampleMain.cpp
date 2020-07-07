#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include <SDL2/SDL.h>

#include "MAngle.hpp"
#include "Component/MCamera.hpp"
#include "Component/MModel.hpp"
#include "Component/MTransformation.hpp"
#include "MEntity.hpp"
#include "MGLRenderSystem.hpp"
#include "MGLShader.hpp"
#include "MGLWindow.hpp"
#include "MMatrix.hpp"
#include "MMeshData.hpp"
#include "MOBJLoader.hpp"
#include "MReadFile.hpp"
#include "MScene.hpp"
#include "MVector.hpp"

using namespace method;

int main() {
    // TODO: (global) path or filename?

    MGLWindow my_window = MGLWindow("method example project", ivec2(800, 600));
    // my_window.grab_cursor(true);
    my_window.set_swap_mode(METHOD_GL_SWAP_MODE_VSYNC);
    MGLRenderSystem my_renderer = MGLRenderSystem(&my_window);

    MGLShader my_shader = MGLShader();
    my_shader.load("example/shader/normal.vert", METHOD_SHADER_VERTEX);
    my_shader.load("example/shader/normal.frag", METHOD_SHADER_FRAGMENT);
    my_shader.compile();

    MMeshData * cube = MOBJLoader::load("example/resource/cube.obj");

    MScene my_scene = MScene();

    MEntity * my_entity = my_scene.emplace();
    my_entity->emplace<MTransformation>();
    //my_entity->get<MTransformation>()->set_scale(vec3(0.01f, 0.01f, 0.01f));
    my_entity->emplace<MModel>(cube);

    MEntity * my_camera = my_scene.emplace();
    my_camera->emplace<MTransformation>();
    ivec2 size = my_window.get_dimensions();
    my_camera->emplace<MCamera>(radians(90.0f), (float)size.x / (float)size.y,
        0.01f, 1000.0f);

    SDL_Event event;
    bool running = true;
    int frame = 0;
    ivec2 mouse_pos;

    float cam_accel = 0.01f;
    vec3 pos, vel, dis, acc, dec;
    float pitch = 0.0f, yaw = 0.0f;

    while (running) {
        frame = (frame + 1) % 360;

        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                else if (event.key.keysym.sym == SDLK_w)
                    acc.z = -cam_accel;
                else if (event.key.keysym.sym == SDLK_s)
                    acc.z = cam_accel;
                else if (event.key.keysym.sym == SDLK_LSHIFT)
                    acc.y = -cam_accel;
                else if (event.key.keysym.sym == SDLK_SPACE)
                    acc.y = cam_accel;
                else if (event.key.keysym.sym == SDLK_a)
                    acc.x = -cam_accel;
                else if (event.key.keysym.sym == SDLK_d)
                    acc.x = cam_accel;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_w)
                    acc.z = 0.0f;
                else if (event.key.keysym.sym == SDLK_s)
                    acc.z = 0.0f;
                else if (event.key.keysym.sym == SDLK_LSHIFT)
                    acc.y = 0.0f;
                else if (event.key.keysym.sym == SDLK_SPACE)
                    acc.y = 0.0f;
                else if (event.key.keysym.sym == SDLK_a)
                    acc.x = 0.0f;
                else if (event.key.keysym.sym == SDLK_d)
                    acc.x = 0.0f;
                break;
            case SDL_MOUSEMOTION:
                SDL_GetRelativeMouseState(&mouse_pos.x, &mouse_pos.y);
                yaw += radians(mouse_pos.x * 0.05f);
                pitch += radians(mouse_pos.y * 0.05f);
                if (pitch > M_PI_2) pitch = M_PI_2;
                if (pitch < -M_PI_2) pitch = -M_PI_2;
                break;
            default:
                break;
            }
        }

        dis = vec3(0.0f, 0.0f, 0.0f) - vel;
        dec = vec3(dis.x / 10, dis.y / 10, dis.z / 10);
        vel = vel + acc + dec;
        pos = pos + vel;

        my_camera->get<MTransformation>()->set_position(pos);
        my_camera->get<MTransformation>()->set_rotation(method::vec3(0.0f,
            1.0f, 0.0f), yaw);

        my_renderer.begin(&my_shader);
        my_renderer.draw(&my_scene);
        my_renderer.end();

    }

    delete cube;
    return 0;
}
