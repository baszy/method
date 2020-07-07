#pragma once

#include <memory>

#include "MComponent.hpp"
#include "MGLVAO.hpp"
#include "MMeshData.hpp"

namespace method {

class MModel : public MComponent {
private:
    MMeshData * m_data;
    MGLVAO m_vao;

public:
    MModel(MMeshData * data);

    MMeshData * get_data();
    MGLVAO * get_vao();

};

}
