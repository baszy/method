#include "Component/MModel.hpp"

namespace method {

MModel::MModel(MMeshData * data)
    : m_data(data)
    , m_vao(data) {}

MMeshData * MModel::get_data() {
    return m_data;
}

MGLVAO * MModel::get_vao() {
    return &m_vao;
}

}
