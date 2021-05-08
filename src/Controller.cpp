#include "Controller.hpp"

#include <SDL2/SDL.h>

namespace method {

// The rest should be default constructed...
Controller::Controller() : running(true) {}

void Controller::update() {
    // TODO: This is kind of ugly
    direction_1 = Vec3(0);
    direction_2 = Vec3(0);
    mouse = IVec2(0);
    mousewheel = IVec2(0);

    const uint8_t * keystate = SDL_GetKeyboardState(NULL);

    running = (keystate[SDL_SCANCODE_ESCAPE]) ? false : true;

    if (keystate[SDL_SCANCODE_S]) direction_1.z = 1;
    else if (keystate[SDL_SCANCODE_W]) direction_1.z = -1;
    if (keystate[SDL_SCANCODE_SPACE]) direction_1.y = 1;
    else if (keystate[SDL_SCANCODE_LSHIFT]) direction_1.y = -1;
    if (keystate[SDL_SCANCODE_D]) direction_1.x = 1;
    else if (keystate[SDL_SCANCODE_A]) direction_1.x = -1;

    if (keystate[SDL_SCANCODE_DOWN]) direction_2.z = 1;
    else if (keystate[SDL_SCANCODE_UP]) direction_2.z = -1;
    if (keystate[SDL_SCANCODE_RCTRL]) direction_2.y = 1;
    else if (keystate[SDL_SCANCODE_RSHIFT]) direction_2.y = -1;
    if (keystate[SDL_SCANCODE_RIGHT]) direction_2.x = 1;
    else if (keystate[SDL_SCANCODE_LEFT]) direction_2.x = -1;

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
        case SDL_MOUSEMOTION:
            mouse.x += event.motion.xrel;
            mouse.y += event.motion.yrel;
            break;
        case SDL_MOUSEWHEEL:
            mousewheel.x = event.wheel.x;
            mousewheel.y = event.wheel.y;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
                mousebutton.x = 1;
            else if (event.button.button == SDL_BUTTON_MIDDLE)
                mousebutton.y = 1;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                mousebutton.z = 1;
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
                mousebutton.x = 0;
            else if (event.button.button == SDL_BUTTON_MIDDLE)
                mousebutton.y = 0;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                mousebutton.z = 0;
            break;
        default:
            break;
        }
    }
}

}
