#pragma once

#include "Buffer.hpp"
#include "View.hpp"
#include "DescriptorSet.hpp"
#include "UniformBuffer.hpp"
#include "Camera2D.hpp"
#include "Pipeline.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include "Mesh/quadMesh.hpp"
#include "Entity/entityPlayer.hpp"
#include "uhcstd.hh"

#include <vulkan/vulkan.h>

class Buffer;

struct ShaderData {
    glm::mat4 projection{1.0f};
    glm::mat4 view{1.0f};
    glm::vec4 viewPosition{0.0f};
    glm::vec4 viewDirection{0.0f};
    float time{0.0f};
};

class GameView: public View {
public:
    GameView();
    ~GameView();

    void onUpdate(double time_since_start, float dt);
    void onDraw(double time_since_start, float dt);
    void onResize(int width, int height);

    void onKeyPress(int key);
    void onMouseMotion(int x, int y, int dx, int dy);

private:
    UniformBuffer m_uniformBuffer{};
    ShaderData m_shaderData{};
    Pipeline m_pipelineSkybox{};
    Pipeline m_pipeline{};
    MapLoader::Map map;
    List::It<QuadMesh::Mesh> m_quadMesh;
    
    EntityPlayer m_player;

    Buffer m_cubemapBufferVertex{};

    Texture gigachad{};
    // Texture m_cubemap{};
    DescriptorSet m_descriptorSet{};
    // DescriptorSet m_descriptorSetCubemap{};

    Camera2D m_camera{};
};
