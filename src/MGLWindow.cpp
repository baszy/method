#include "MGLWindow.hpp"

#include <iostream>
#include <stdexcept>

#include "glad/glad.h"

namespace method {

MGLWindow::MGLWindow()
    : MGLWindow(METHOD_DEFAULT_WINDOW_TITLE, ivec2(640, 480)) {}

MGLWindow::MGLWindow(const std::string & title, const ivec2 & dimensions)
    : MRenderTarget(dimensions) {

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error("SDL_InitSubSystem failed with subsystems: "
            "SDL_INIT_VIDEO");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_sdl_window = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        m_dimensions.x, m_dimensions.y, SDL_WINDOW_OPENGL);

    if(m_sdl_window == nullptr) {
        throw std::runtime_error("SDL_Window could not be created");
    }

    m_sdl_glcontext = SDL_GL_CreateContext(m_sdl_window);

    if (m_sdl_glcontext == nullptr) {
        throw std::runtime_error("SDL_GLContext could not be created");
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        throw std::runtime_error("glad could not be initialized");
    }
}

MGLWindow::~MGLWindow() {
    SDL_GL_DeleteContext(m_sdl_glcontext);
    SDL_DestroyWindow(m_sdl_window);
    SDL_Quit();
}

ivec2 MGLWindow::get_viewport() {
    ivec2 result;
    SDL_GL_GetDrawableSize(m_sdl_window, &result.x, &result.y);
    return result;
}

void MGLWindow::grab_cursor(bool enable) {
    SDL_SetRelativeMouseMode(enable ? SDL_TRUE : SDL_FALSE);
    SDL_WarpMouseInWindow(m_sdl_window, m_dimensions.x / 2, m_dimensions.y / 2);
}

void MGLWindow::set_fullscreen(bool enable) {
    SDL_SetWindowFullscreen(m_sdl_window, enable ? SDL_WINDOW_FULLSCREEN : 0);
}

void MGLWindow::set_position(ivec2 position) {
    SDL_SetWindowPosition(m_sdl_window, position.x, position.y);
}

void MGLWindow::set_resizeable(bool enable) {
    SDL_SetWindowResizable(m_sdl_window, enable ? SDL_TRUE : SDL_FALSE);
}

void MGLWindow::set_swap_mode(MGLSwapMode mode) {
    SDL_GL_SetSwapInterval(mode);
}

void MGLWindow::swap() {
    SDL_GL_SwapWindow(m_sdl_window);
}

}