#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "MComponent.hpp"

namespace method {

class MEntity {
private:
    std::unordered_map<std::type_index, std::shared_ptr<MComponent>> m_components;

    template <typename Type>
    std::type_index get_type_index() const {
        return std::type_index(typeid(Type));
    }

public:
    void clear();

    template <typename ... Types, typename Func>
    void call(Func function) {
        if (has<Types ...>())
            function(get<Types>() ...);
    }

    template <typename Type, typename ... Args>
    void emplace(Args &&... arguments) {
        m_components.emplace(get_type_index<Type>(),
            std::make_shared<Type>(std::forward<Args>(arguments) ...));
    }

    template <typename Type>
    bool erase() {
        return m_components.erase(get_type_index<Type>());
    }

    template <typename Type>
    std::shared_ptr<Type> get() const {
        return std::static_pointer_cast<Type>(
            m_components.at(get_type_index<Type>()));
    }

    template <typename Type>
    bool has() const {
        std::type_index index = get_type_index<Type>();
        return m_components.find(index) != m_components.end();
    }

    template <typename Type1, typename Type2, typename ... Types>
    bool has() const {
        return has<Type1>() && has<Type2, Types ...>();
    }

    template <typename Type>
    void insert(const Type & component) {
        m_components.insert(get_type_index<Type>(),
            std::make_shared<Type>(component));
    }

};

}