#pragma once

#include "MVector.hpp"

namespace method {

class MRenderTarget {
protected:
    ivec2 m_dimensions;

public:
    MRenderTarget();
    MRenderTarget(const ivec2 & dimensions);
    virtual ~MRenderTarget();

    ivec2 get_dimensions() const;
    virtual void swap();
    virtual void use();

};

}