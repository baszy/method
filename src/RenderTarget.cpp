#include "RenderTarget.hpp"

#include <iostream>
#include <stdexcept>

#include "glad/glad.h"

namespace method {

RenderTarget::RenderTarget()
    : RenderTarget(METHOD_DEFAULT_WINDOW_TITLE, IVec2(640, 480)) {}

RenderTarget::RenderTarget(const std::string & title,
    const IVec2 & dimensions) {

    m_dimensions = dimensions;

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

RenderTarget::~RenderTarget() {
    SDL_GL_DeleteContext(m_sdl_glcontext);
    SDL_DestroyWindow(m_sdl_window);
    SDL_Quit();
}

IVec2 RenderTarget::get_dimensions() const {
    return m_dimensions;
}

IVec2 RenderTarget::get_viewport() {
    IVec2 result;
    SDL_GL_GetDrawableSize(m_sdl_window, &result.x, &result.y);
    return result;
}

void RenderTarget::set_fullscreen(bool enable) {
    SDL_SetWindowFullscreen(m_sdl_window, enable ? SDL_WINDOW_FULLSCREEN : 0);
}

void RenderTarget::set_position(IVec2 position) {
    SDL_SetWindowPosition(m_sdl_window, position.x, position.y);
}

void RenderTarget::set_resizeable(bool enable) {
    SDL_SetWindowResizable(m_sdl_window, enable ? SDL_TRUE : SDL_FALSE);
}

void RenderTarget::set_swap_mode(SwapMode mode) {
    SDL_GL_SetSwapInterval(static_cast<int>(mode));
}

void RenderTarget::swap() {
    SDL_GL_SwapWindow(m_sdl_window);
}

void RenderTarget::use() {}

}
