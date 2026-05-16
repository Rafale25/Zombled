#include "quadMesh.hpp"

namespace QuadMesh {

    Mesh create(int x, int y, int w, int h) {
        Mesh mesh;
    
        const VkDeviceSize indexCount{6};
        std::vector<Vertex> vertices{
            {{-1 + x,  1 + y + h, 0}, {1, 0, 0}, {0, 0}, 0},
            {{ 1 + x + w,  1 + y + h, 0}, {0, 1, 0}, {1, 0}, 0},
            {{-1 + x, -1 + y, 0}, {0, 0, 1}, {0, 1}, 0},
            {{ 1 + x + w, -1 + y, 0}, {0, 1, 1}, {1, 1}, 0},
        };
        std::vector<uint16_t> indices{
            0, 1, 2,
            2, 1, 3,
        };

        mesh.verticesSize = vertices.size() * sizeof(Vertex);
        mesh.indicesSize = indices.size() * sizeof(uint16_t);

        mesh.m_bufferVertex.create(mesh.verticesSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        mesh.m_bufferIndices.create(mesh.indicesSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

        mesh.m_bufferVertex.upload(vertices.data(), mesh.verticesSize);
        mesh.m_bufferIndices.upload(indices.data(), mesh.indicesSize);

        return mesh;
    }
}
