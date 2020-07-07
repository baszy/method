#pragma once

#include <iostream>
#include <vector>

#include "glad/glad.h"

#include "MMeshData.hpp"

namespace method {

class MGLVAO {
private:
    GLuint m_num_vbos;
    GLuint m_vao_id;

    template <typename Type>
    void vbo_data(const std::vector<Type> * data) {
        GLuint vbo_id;

        glGenBuffers(1, &vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(Type),
            data->data(), GL_STATIC_DRAW);

        // TODO: assumes float
        glVertexAttribPointer(m_num_vbos, sizeof(Type) / sizeof(float),
            GL_FLOAT, GL_FALSE, sizeof(Type), (void*)(0));
        glEnableVertexAttribArray(m_num_vbos);

        m_num_vbos++;
    }

public:
    // TODO: why why why why why is this a pointer
    MGLVAO(const MMeshData * mesh);

    GLuint get_id();

};

}
