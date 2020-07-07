#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "MRenderTarget.hpp"
#include "MVector.hpp"

#define METHOD_DEFAULT_WINDOW_TITLE "method"

namespace method {

enum MGLSwapMode {
    METHOD_GL_SWAP_MODE_ADAPTIVE = -1,
    METHOD_GL_SWAP_MODE_IMMEDIATE = 0,
    METHOD_GL_SWAP_MODE_VSYNC = 1
};

class MGLWindow : public MRenderTarget {
private:
    SDL_Window * m_sdl_window;
    SDL_GLContext m_sdl_glcontext;

public:
    MGLWindow();
    MGLWindow(const std::string & title, const ivec2 & dimensions);
    ~MGLWindow();

    ivec2 get_viewport();
    void grab_cursor(bool enable);
    void set_fullscreen(bool enable);
    void set_position(ivec2 position);
    void set_resizeable(bool enable);
    void set_swap_mode(MGLSwapMode mode);
    void swap() override;

};

}