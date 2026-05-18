#include "Menu.hpp"
#include "Game.hpp"
#include "Context.hpp"
#include "GameState.hpp"
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

    m_shaderData.time = glm::mod(glfwGetTime(), 1000.0);
}

void MenuView::onDraw(double time_since_start, float dt) {
    VkCommandBuffer cb = Context::instance().getCommandBuffer();
    Context& ctx = Context::instance();

    m_uniformBuffer.upload(&m_shaderData, sizeof(ShaderData));

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

        m_uniformBuffer.pushConstant(m_pipeline.layout, VkShaderStageFlagBits(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT));

        vkCmdBindVertexBuffers(cb, 0, 1, &m_bufferVertex.buffer, &_vOffset);
        vkCmdBindIndexBuffer(cb, m_bufferIndices.buffer, 0, VK_INDEX_TYPE_UINT16);
        const VkDeviceSize indexCount{ 6 };
        vkCmdDrawIndexed(cb, indexCount, 1, 0, 0, 0);
    });

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
            connectToServer();
        }

    ImGui::End();

    ImGui::ShowDemoWindow();
}

void* networkThread(void* arg) {
    using namespace Zombled::Packets;
    logI("Network Thread Start");

    TcpClient::It client = *(TcpClient::It*)arg;

    char buffer[1024] = {};

    while (client.sockfd) {
        TcpClient::readAll(client, buffer, 1);
        uint8_t value = buffer[0];
        Server::PacketId packetId = (Server::PacketId)value;

        uint8_t size = Server::packetsSize.at(packetId);
        TcpClient::readAll(client, buffer + 1, size - 1);

        UhcBuffer::It buf{};
        buf.handle = buffer;
        UhcBuffer::setOrder(buf, UHC_BIG_ENDIAN);

        switch (packetId) {
            case Server::PacketId::IDENTIFICATION:
            {
                Server::Identification packet;
                memcpy(&packet, buffer, sizeof(packet));
                logD("Packet: IDENTIFICATION {}, {}", packet.id, packet.entityId);
                break;
            }
            case Server::PacketId::ENTITY_ADD:
            {
                Server::EntityAdd packet;

                UhcBuffer::reset(buf);
                packet.id = UhcBuffer::getU8(buf);
                packet.entityId = UhcBuffer::getU32(buf);
                packet.entityType = UhcBuffer::getU32(buf);
                packet.x = UhcBuffer::getF32(buf);
                packet.y = UhcBuffer::getF32(buf);
                packet.z = UhcBuffer::getF32(buf);
                packet.rot = UhcBuffer::getF32(buf);
                // packet.name = UhcBuffer::get(buf);

                // if (packet.entityType == 0)

                g_gameState.entities[packet.entityId] = std::make_unique<EntityPlayer>();

                Entity& player = *g_gameState.entities.at(packet.entityId);
                player.id = packet.entityId;
                player.position = glm::vec2(packet.x, packet.y);
                break;
            }
            case Server::PacketId::ENTITY_REMOVE:
            {
                Server::EntityRemove packet;

                UhcBuffer::reset(buf);
                packet.id = UhcBuffer::getU8(buf);
                packet.entityId = UhcBuffer::getU32(buf);

                g_gameState.entities.erase(packet.entityId);
                break;
            }
            case Server::PacketId::ENTITY_MOVE:
            {
                Server::EntityMove packet;

                UhcBuffer::reset(buf);
                packet.id = UhcBuffer::getU8(buf);
                packet.entityId = UhcBuffer::getU32(buf);
                packet.x = UhcBuffer::getF32(buf);
                packet.y = UhcBuffer::getF32(buf);
                packet.z = UhcBuffer::getF32(buf);
                packet.rot = UhcBuffer::getF32(buf);

                Entity& e = *g_gameState.entities.at(packet.entityId);
                e.position = glm::vec2(packet.x, packet.y);
                e.rot = packet.rot;
                break;
            }
        }
    }

    return nullptr;
}

void MenuView::connectToServer() {
    uint16_t port = 22222;

    g_gameState.networkClient = TcpClient::create(buffer_ip, port);
    if (g_gameState.networkClient.sockfd != -1) {
        static GameView gameView;
        Context::instance().viewPush(gameView);

        logI("Successfuly connected to {}:{}", buffer_ip, port);
        g_gameState.networkThread = Thread::start(networkThread, &g_gameState.networkClient);

    } else {
        logE("Failed to connect to {}:{}", buffer_ip, port);
    }
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
