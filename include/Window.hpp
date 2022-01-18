#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "Framebuffer.hpp"
#include "Shader.hpp"
#include "Vector.hpp"

#define METHOD_DEFAULT_WINDOW_TITLE "method"

namespace method {

enum SwapMode {
    ADAPTIVE = -1,
    IMMEDIATE = 0,
    VSYNC = 1
};

class Window {
private:
    GLuint screen_vao;

protected:
    IVec2 m_dimensions;

    SDL_Window * m_sdl_window;
    SDL_GLContext m_sdl_glcontext;

    Shader post_shader;

public:
    Window();
    Window(const std::string & title, const IVec2 & dimensions);
    ~Window();

    IVec2 get_dimensions() const;
    IVec2 get_viewport();
    void grab_cursor(bool enable);
    void present(const Framebuffer & fb1);
    void set_fullscreen(bool enable);
    void set_position(IVec2 position);
    void set_resizeable(bool enable);
    void set_swap_mode(SwapMode mode);
    void swap();
};

}
