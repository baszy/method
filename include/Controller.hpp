#pragma once

#include "Vector.hpp"

namespace method {

class Controller {
public:
    // TODO: Bool packing sucks
    bool running;
    Vec3 direction_axis;
    IVec2 mouse_axis;
    bool test_axis;
    
    Controller();

    /// \brief Lets the window grab the cursor
    void grab_cursor(bool enable);

    /// \brief Updates this Controller's axises based on the current input
    void update();
};

}
