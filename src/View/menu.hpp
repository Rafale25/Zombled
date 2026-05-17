#pragma once

#include "Buffer.hpp"
#include "View.hpp"
#include "UniformBuffer.hpp"
#include "Pipeline.hpp"
#include <vulkan/vulkan.h>
#include "ShaderData.hpp"

class Buffer;

class MenuView: public View {
public:
    MenuView();
    ~MenuView();

    void onUpdate(double time_since_start, float dt);
    void onDraw(double time_since_start, float dt);
    void onResize(int width, int height);

    void onKeyPress(int key);
    void onMouseMotion(int x, int y, int dx, int dy);

private:
    UniformBuffer m_uniformBuffer{};
    ShaderData m_shaderData{};
    Pipeline m_pipeline{};

    Buffer m_bufferVertex{};
    Buffer m_bufferIndices{};
};
