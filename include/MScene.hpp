#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "MEntity.hpp"

namespace method {

class MScene {
private:
    std::vector<MEntity> m_entities;

public:
    template <typename ... Args>
    MEntity * emplace(Args &&... arguments) {
        m_entities.emplace_back(std::forward<Args>(arguments) ...);
        return &m_entities.back();
    }

    template <typename ... Types, typename Func>
    void for_each(Func function) {
        for (MEntity & entity : m_entities) {
            entity.call<Types ...>(function);
        }
    }

    void insert(const MEntity & entity);

};

}