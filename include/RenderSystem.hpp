#pragma once

#include <string>

#include "RenderTarget.hpp"
#include "Scene.hpp"
#include "GLShader.hpp"

namespace method {

class RenderSystem {
protected:
    RenderTarget * m_target;

public:
    RenderSystem(RenderTarget * target);
    ~RenderSystem();

    void begin();
    void draw(const Scene & scene);
    void end();
    std::string get_info() const;
    RenderTarget * get_render_target();

};

}
