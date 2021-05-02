#include "Window.hpp"

#include <iostream>

#include "glad/glad.h"

#include "GLDebugCallback.hpp"

namespace method {

namespace {

Vec3 quad_vertices[6] = {
    Vec3(1.0f, 1.0f, 0.0f), Vec3(-1.0f, 1.0f, 0.0f), Vec3(-1.0f, -1.0f, 0.0f),
    Vec3(1.0f, 1.0f, 0.0f), Vec3(-1.0f, -1.0f, 0.0f), Vec3(1.0f, -1.0f, 0.0f)
};

Vec2 quad_uvs[6] = {
    Vec2(1.0f, 1.0f), Vec2(0.0f, 1.0f), Vec2(0.0f, 0.0f),
    Vec2(1.0f, 1.0f), Vec2(0.0f, 0.0f), Vec2(1.0f, 0.0f)
};

const char * vertex_source = METHOD_GLSL(
    layout (location = 0) in vec2 position;
    layout (location = 1) in vec2 uv;

    out vec2 texture_coord;

    void main()
    {
        texture_coord = uv;
        gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    }
);

const char * fragment_source = METHOD_GLSL(
    in vec2 texture_coord;

    uniform sampler2D frame;

    out vec4 frag_color;

    const float o = 1.0f / 100.0f;
    float kernel[9] = float[](
        1.0 / 9, 1.0 / 9, 1.0 / 9,
        1.0 / 9, 1.0 / 9, 1.0 / 9,
        1.0 / 9, 1.0 / 9, 1.0 / 9
    );

    void main()
    {
        vec3 col = texture(frame, texture_coord).rgb;

        /*
        for (int i = 0; i < 1; i++)
            for (int y = -1; y <= 1; y++)
                for (int x = -1; x <= 1; x++)
                    col += kernel[(x + 1) + (y + 1) * 3]
                           * texture(frame, texture_coord + vec2(x * o, y * o)).rgb;
        */

        frag_color = vec4(col, 1.0);
    }
);

}

Window::Window()
    : Window(METHOD_DEFAULT_WINDOW_TITLE, IVec2(640, 480)) {}

Window::Window(const std::string & title, const IVec2 & dimensions)
    : m_dimensions(dimensions)
    , post_shader() {

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_InitSubSystem failed with subsystems: "
                  << "SDL_INIT_VIDEO" << std::endl;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_sdl_window = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        m_dimensions.x, m_dimensions.y, SDL_WINDOW_OPENGL);

    if(m_sdl_window == nullptr) {
        std::cout << "SDL_Window could not be created" << std::endl;
    }

    m_sdl_glcontext = SDL_GL_CreateContext(m_sdl_window);

    if (m_sdl_glcontext == nullptr) {
        std::cout << "SDL_GLContext could not be created" << std::endl;
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "glad could not be initialized" << std::endl;
    }

    glDebugMessageCallback(&gl_debug_callback, nullptr);
    glEnable(GL_DEBUG_OUTPUT);

    // Screen quad vao
    glGenVertexArrays(1, &screen_vao);
    glBindVertexArray(screen_vao);

    GLuint vbos[2];
    glGenBuffers(2, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vec3), quad_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vec2), quad_uvs, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)(0));
    glEnableVertexAttribArray(1);

    // TODO: useless?
    glBindVertexArray(0);

    // Setting up the post-processing shader
    post_shader.set_source(vertex_source, ShaderType::VERTEX);
    post_shader.set_source(fragment_source, ShaderType::FRAGMENT);
    post_shader.compile();
}

Window::~Window() {
    SDL_GL_DeleteContext(m_sdl_glcontext);
    SDL_DestroyWindow(m_sdl_window);
    SDL_Quit();
}

void Window::draw_framebuffer(const Framebuffer & framebuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    post_shader.use();

    post_shader.set_uniform("frame", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebuffer.color_buffer_id);

    glBindVertexArray(screen_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

IVec2 Window::get_dimensions() const {
    return m_dimensions;
}

IVec2 Window::get_viewport() {
    IVec2 result;
    SDL_GL_GetDrawableSize(m_sdl_window, &result.x, &result.y);
    return result;
}

void Window::set_fullscreen(bool enable) {
    SDL_SetWindowFullscreen(m_sdl_window, enable ? SDL_WINDOW_FULLSCREEN : 0);
}

void Window::set_position(IVec2 position) {
    SDL_SetWindowPosition(m_sdl_window, position.x, position.y);
}

void Window::set_resizeable(bool enable) {
    SDL_SetWindowResizable(m_sdl_window, enable ? SDL_TRUE : SDL_FALSE);
}

void Window::set_swap_mode(SwapMode mode) {
    SDL_GL_SetSwapInterval(static_cast<int>(mode));
}

void Window::swap() {
    SDL_GL_SwapWindow(m_sdl_window);
}

void Window::use() {}

}
