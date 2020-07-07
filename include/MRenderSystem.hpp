#pragma once

#include <string>

#include "MRenderTarget.hpp"
#include "MScene.hpp"
#include "MShader.hpp"

#define METHOD_DEFAULT_RENDER_INFO "Generic render system"

namespace method {

class MRenderSystem {
protected:
    MRenderTarget * m_target;
    MShader * m_shader;

public:
    MRenderSystem(MRenderTarget * target);
    virtual ~MRenderSystem();

    virtual void begin(MShader * shader);
    virtual void draw(MScene * scene);
    virtual void end();
    virtual std::string get_info() const;
    MRenderTarget * get_render_target();

};

}