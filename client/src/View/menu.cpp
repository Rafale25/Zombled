#include "Menu.hpp"
#include "Game.hpp"
#include "Context.hpp"
#include "UniformBuffer.hpp"
#include "RenderPass.hpp"
#include "GraphicsPipelineBuilder.hpp"
#include "Logger.hpp"
#include "geometry.hpp"
#include <glm/ext/vector_float3.hpp>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "uhcnet.hh"


MenuView::~MenuView() {
    m_pipeline.destroy();
}

MenuView::MenuView() {
    const Context& ctx = Context::instance();

    m_uniformBuffer.create(sizeof(ShaderData));

    m_pipeline = GraphicsPipelineBuilder{}
        .setShaders("menuBackground", ASSETS_PATH "menuBackground.slang")
        .addColorAttachmentFormat(Context::SWAPCHAIN_IMAGE_FORMAT)
        .setDepthAttachmentFormat(ctx.getDepthImageFormat())
        .addVertexBinding(0, sizeof(float) * 5)
        // .addDescriptorLayout(m_descriptorSet.getLayout())
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 0)
        .addVertexAttribute(1, 0, VK_FORMAT_R32G32_SFLOAT,    sizeof(float) * 3)
        .setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
        .setPolygonMode(VK_POLYGON_MODE_FILL)
        .build();

    Geometry::quadFullscreen(m_bufferVertex, m_bufferIndices);
}

void MenuView::onUpdate(double time_since_start, float dt) {
    const Context& ctx = Context::instance();
}

void MenuView::onDraw(double time_since_start, float dt) {
    VkCommandBuffer cb = Context::instance().getCommandBuffer();
    Context& ctx = Context::instance();

    auto pass = RenderPass()
        .defaultViewportScissor()
        .color(
            ctx.getSwapchainImage(),
            ctx.getSwapchainImageView(),
            { .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR })
        .depth(
            ctx.getDepthTexture(),
            { .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR });

    pass.execute([&]() {
        VkDeviceSize _vOffset{ 0 };
        vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.pipeline);
        vkCmdBindVertexBuffers(cb, 0, 1, &m_bufferVertex.buffer, &_vOffset);
        vkCmdBindIndexBuffer(cb, m_bufferIndices.buffer, 0, VK_INDEX_TYPE_UINT16);
        const VkDeviceSize indexCount{ 6 };
        vkCmdDrawIndexed(cb, indexCount, 1, 0, 0, 0);
    });

    static char buffer_username[256] = {};
    static char buffer_ip[256] = {};

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoBackground;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoSavedSettings;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;

    ImGui::SetNextWindowPos((ImVec2){0, 0});
    ImGui::SetNextWindowSize((ImVec2){400, 600});

    ImGui::Begin("##FullscreenWindow", nullptr, window_flags);
        ImGui::InputText("username", buffer_username, sizeof(buffer_username));
        ImGui::InputText("ip", buffer_ip, sizeof(buffer_ip));

        if (ImGui::Button("Join")) {

            // TcpClient::create("127.0.0.1", 22222);
            // logD("Joining...");
            static GameView gameView;
            ctx.viewPush(gameView);
        }

    ImGui::End();

    ImGui::ShowDemoWindow();
}

void MenuView::onKeyPress(int key) {
    auto& ctx = Context::instance();

    if (key == GLFW_KEY_C && !ImGui::GetIO().WantCaptureKeyboard) {
        ctx.toggleCursor();
    }
}

void MenuView::onMouseMotion(int x, int y, int dx, int dy) {
}

void MenuView::onResize(int width, int height) {
}
