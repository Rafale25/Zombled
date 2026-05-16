namespace QuadMesh {

    Mesh create() {
        const VkDeviceSize indexCount{6};
        std::vector<Vertex> vertices{
            {{-1,  1, -5}, {1, 0, 0}, {0, 0}}, 0,
            {{ 1,  1, -5}, {0, 1, 0}, {1, 0}}, 0,
            {{-1, -1, -5}, {0, 0, 1}, {0, 1}}, 0,
            {{ 1, -1, -5}, {0, 1, 1}, {1, 1}}, 0,
        };
        std::vector<uint16_t> indices{
            0, 1, 2,
            2, 1, 3,
        };

        int verticesSize = vertices.size() * sizeof(Vertex);
        int indicesSize = indices.size() * sizeof(uint16_t);

        m_bufferVertex.create(verticesSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        m_bufferIndices.create(indicesSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

        m_bufferVertex.upload(vertices.data(), verticesSize);
        m_bufferIndices.upload(indices.data(), indicesSize);
    }
}
