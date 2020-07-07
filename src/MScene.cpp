#include "MScene.hpp"

namespace method {

void MScene::insert(const MEntity & entity) {
    m_entities.push_back(entity);
}

}