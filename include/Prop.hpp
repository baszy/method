#pragma once

#include "Entity.hpp"
#include "GLVAO.hpp"
#include "Material.hpp"
#include "MeshData.hpp"

namespace method {

class Prop : public Entity {
private:
    Material * m_material;
    MeshData * m_data;
    GLVAO m_vao;

public:
    Prop(MeshData * data, Material * material);
    
    MeshData * get_data() const;
    Material * get_material() const;
    const GLVAO * get_vao() const;

};

}
