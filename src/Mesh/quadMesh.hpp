#pragma once

#include <glm/glm.hpp>

namespace QuadMesh {
    struct Mesh {
        Buffer m_bufferVertex{};
        Buffer m_bufferIndices{};
        int verticesSize = 0;
        int indicesSize = 0;
    };

    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 uv;
        int index;
    };

    Mesh create();
}
