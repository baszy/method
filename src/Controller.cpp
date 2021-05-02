#include "Controller.hpp"

#include <SDL2/SDL.h>

namespace method {

Controller::Controller() : running(true),
                           direction_axis(Vec3(0.0f, 0.0f, 0.0f)),
                           mouse_axis(IVec2(0, 0)) {}

void Controller::grab_cursor(bool enable) {
    SDL_SetRelativeMouseMode(enable ? SDL_TRUE : SDL_FALSE);
}

void Controller::update() {
    direction_axis = Vec3(0.0f, 0.0f, 0.0f);
    mouse_axis = IVec2(0, 0);

    const uint8_t * keystate = SDL_GetKeyboardState(NULL);
    this->running = (keystate[SDL_SCANCODE_ESCAPE]) ? false : true;
    this->test_axis = (keystate[SDL_SCANCODE_R]) ? false : true;
    this->direction_axis.z = (keystate[SDL_SCANCODE_W]) ? -1 : this->direction_axis.z;
    this->direction_axis.z = (keystate[SDL_SCANCODE_S]) ? 1 : this->direction_axis.z;
    this->direction_axis.y = (keystate[SDL_SCANCODE_LSHIFT]) ? -1 : this->direction_axis.y;
    this->direction_axis.y = (keystate[SDL_SCANCODE_SPACE]) ? 1 : this->direction_axis.y;
    this->direction_axis.x = (keystate[SDL_SCANCODE_A]) ? -1 : this->direction_axis.x;
    this->direction_axis.x = (keystate[SDL_SCANCODE_D]) ? 1 : this->direction_axis.x;

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
        case SDL_MOUSEMOTION:
            this->mouse_axis.x += event.motion.xrel;
            this->mouse_axis.y += event.motion.yrel;
            break;
        default:
            break;
        }
    }
}

}
