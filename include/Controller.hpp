#pragma once

#include "Vector.hpp"

namespace method {

class Controller {
public:
    // TODO: Bool packing sucks
    bool running;
    // Directional axes (like Analog stick, WASD, arrow keys)
    Vec3 direction_1;
    Vec3 direction_2;
    // Mouse axes (for mouse move, scroll, and click)
    IVec2 mouse;
    IVec2 mousewheel;
    IVec3 mousebutton;

    Controller();

    /** \brief Updates this Controller's axes based on keyboard input */
    void update();
};

}
