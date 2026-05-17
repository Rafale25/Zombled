#pragma once

#include "Buffer.hpp"
#include "View.hpp"
#include "DescriptorSet.hpp"
#include "UniformBuffer.hpp"
#include "Camera2D.hpp"
#include "Pipeline.hpp"
#include "ShaderData.hpp"
#include "Mesh/quadMesh.hpp"
#include "Entity/entityPlayer.hpp"
#include "uhcstd.hh"

#include <vulkan/vulkan.h>

class Buffer;

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

    std::vector<Texture> m_textures;
    DescriptorSet m_descriptorSet{};

    Camera2D m_camera{};
};
