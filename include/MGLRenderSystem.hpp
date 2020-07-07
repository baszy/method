#pragma once

#include "MGLShader.hpp"
#include "MGLWindow.hpp"
#include "MRenderSystem.hpp"
#include "MScene.hpp"

namespace method {

class MGLRenderSystem : public MRenderSystem {
public:
    MGLRenderSystem(MGLWindow * target);

    void begin(MShader * shader) override;
    void draw(MScene * world) override;
    std::string get_info() const override;

};

}