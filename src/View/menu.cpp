#include "Context.hpp"
#include "menu.hpp"
#include "UniformBuffer.hpp"
#include "RenderPass.hpp"
#include "GraphicsPipelineBuilder.hpp"
#include "Logger.hpp"
#include "geometry.hpp"
#include "debugdraw/DebugDraw.hpp"
#include <glm/ext/vector_float3.hpp>
#include <GLFW/glfw3.h>
#include "imgui.h"

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 uv;
};

MenuView::~MenuView() {
    m_pipeline.destroy();
    m_pipelineSkybox.destroy();
    gigachad.destroy();
}

MenuView::MenuView() {
    const Context& ctx = Context::instance();

    m_uniformBuffer.create(sizeof(ShaderData));

    m_camera = { CameraFpsCreateInfo{
        .aspect_ratio= ctx.aspectRatio(),
        .fov = 70.0f,
        .yaw = -glm::pi<float>()/2.0f,
        .nearPlane=0.1,
        .farPlane=1000.0,
        .isScreenYInverted = true
    }};
}

void MenuView::onUpdate(double time_since_start, float dt) {
    const Context& ctx = Context::instance();

    m_camera.update(dt);

    glm::vec3 delta = {
        ctx.isKeyDown(GLFW_KEY_A)            - ctx.isKeyDown(GLFW_KEY_D),
        ctx.isKeyDown(GLFW_KEY_LEFT_CONTROL) - ctx.isKeyDown(GLFW_KEY_SPACE),
        ctx.isKeyDown(GLFW_KEY_W)            - ctx.isKeyDown(GLFW_KEY_S)
    };
    if (!ctx.isCursorEnabled() && !ImGui::GetIO().WantCaptureKeyboard)
        m_camera.move(delta);

    m_shaderData.projection = m_camera.getProjection();
    m_shaderData.view = m_camera.getView();
    m_shaderData.viewPosition = glm::vec4(m_camera.getPosition(), 0);
    m_shaderData.viewDirection = glm::vec4(m_camera.forward(), 0);
    m_shaderData.time = time_since_start;
}

void MenuView::onDraw(double time_since_start, float dt) {
    VkCommandBuffer cb = Context::instance().getCommandBuffer();
    Context& ctx = Context::instance();

    m_uniformBuffer.upload(&m_shaderData, sizeof(ShaderData));

    DebugDraw::instance().drawCube({0, 0, 0});
    DebugDraw::instance().drawAndFlush(cb, m_shaderData.projection * m_shaderData.view);

    ImGui::ShowDemoWindow();
}

void MenuView::onKeyPress(int key) {
    auto& ctx = Context::instance();

    if (key == GLFW_KEY_C && !ImGui::GetIO().WantCaptureKeyboard) {
        ctx.toggleCursor();
    }
}

void MenuView::onMouseMotion(int x, int y, int dx, int dy) {
    if (!Context::instance().isCursorEnabled())
        m_camera.onMouseMotion(x, y, dx, dy);
}

void MenuView::onResize(int width, int height) {
    m_camera.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}
