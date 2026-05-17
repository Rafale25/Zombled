#include "Context.hpp"
#include "game.hpp"
#include "UniformBuffer.hpp"
#include "RenderPass.hpp"
#include "GraphicsPipelineBuilder.hpp"
#include "Logger.hpp"
#include "geometry.hpp"
#include "debugdraw/DebugDraw.hpp"
#include <glm/ext/vector_float3.hpp>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "MapLoader/mapLoader.hpp"
#include "Mesh/quadMesh.hpp"

GameView::~GameView() {
    m_pipeline.destroy();
    m_pipelineSkybox.destroy();
    gigachad.destroy();
}

GameView::GameView() {
    const Context& ctx = Context::instance();

    map = MapLoader::load(ASSETS_PATH "collision.2de");
    for (int i = 0; i < (int)List::size(map.layers); i++) {
        MapLoader::MapLayer l = *List::get(map.layers, i);

        for (int j = 0; j < (int)List::size(l.tiles); j++) {
            MapLoader::MapTile mt = *List::get(l.tiles, j);
            List::add(m_quadMesh, QuadMesh::create(mt.x, mt.y, mt.width, mt.height));
        }
    }

    m_uniformBuffer.create(sizeof(ShaderData));
    gigachad.createFromFile(ASSETS_PATH "texture.png");

    gigachad.createSampler(VK_FILTER_NEAREST, VK_FILTER_NEAREST);


    // const char* paths[6] = {
    //     "./src/skybox/right.jpg",   // +X
    //     "./src/skybox/left.jpg",    // -X
    //     "./src/skybox/top.jpg",     // +Y
    //     "./src/skybox/bottom.jpg",  // -Y
    //     "./src/skybox/front.jpg",   // +Z
    //     "./src/skybox/back.jpg",    // -2
    // };

    // m_cubemap.createFromFileCubemap(paths);
    // m_cubemap.createSampler();

    // m_descriptorSetCubemap.create(0, 1);
    // m_descriptorSetCubemap.addTexture(m_cubemap.sampler, m_cubemap.imageView);

    m_descriptorSet.create(0, 1);
    int32_t _textureIndex = m_descriptorSet.addTexture(gigachad);


    Geometry::cube(m_cubemapBufferVertex, glm::vec3(100.0f), glm::vec3(0.0f));

    // m_pipelineSkybox = GraphicsPipelineBuilder{}
    //     .setShaders("skybox", "./src/skybox.slang")
    //     .addColorAttachmentFormat(Context::SWAPCHAIN_IMAGE_FORMAT)
    //     .setDepthAttachmentFormat(ctx.getDepthImageFormat())
    //     .addVertexBinding(0, sizeof(float) * 3)
    //     .addDescriptorLayout(m_descriptorSetCubemap.getLayout())
    //     .addVertexAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0)
    //     .setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
    //     .setPolygonMode(VK_POLYGON_MODE_FILL)
    //     .build();

    m_pipeline = GraphicsPipelineBuilder{}
        .setShaders("triangle", ASSETS_PATH "shader.slang")
        .addColorAttachmentFormat(Context::SWAPCHAIN_IMAGE_FORMAT)
        .setDepthAttachmentFormat(ctx.getDepthImageFormat())
        .addVertexBinding(0, sizeof(QuadMesh::Vertex))
        .addDescriptorLayout(m_descriptorSet.getLayout())
        .addVertexAttribute(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(QuadMesh::Vertex, pos))
        .addVertexAttribute(1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(QuadMesh::Vertex, color))
        .addVertexAttribute(2, 0, VK_FORMAT_R32G32_SFLOAT,    offsetof(QuadMesh::Vertex, uv))
        .addVertexAttribute(3, 0, VK_FORMAT_R32_SINT,         offsetof(QuadMesh::Vertex, index))
        .setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
        .setPolygonMode(VK_POLYGON_MODE_FILL)
        .build();

    m_camera = Camera2D({0, 0}, 0, ctx.m_framebufferWidth, 0, ctx.m_framebufferHeight);
    m_player = EntityPlayer({0, 0}, {32.0f, 32.0f});
}

void GameView::onUpdate(double time_since_start, float dt) {
    const Context& ctx = Context::instance();

    glm::vec3 delta = {
        ctx.isKeyDown(GLFW_KEY_A)            - ctx.isKeyDown(GLFW_KEY_D),
        ctx.isKeyDown(GLFW_KEY_W)            - ctx.isKeyDown(GLFW_KEY_S),
        0
    };

    m_shaderData.projection = m_camera.getProjection();
    m_shaderData.view = m_camera.getView();
    m_shaderData.viewPosition = glm::vec4(m_camera.getPosition(), 0);
    m_shaderData.viewDirection = glm::vec4(m_camera.forward(), 0);
    m_shaderData.time = time_since_start;

    m_player.velocity.x -= delta.x;
    m_player.velocity.y -= delta.y;

    if (glm::length(m_player.velocity) > 0.001f)
        m_player.velocity = glm::normalize(m_player.velocity) * m_player.speed;

    m_player.update(dt, map);


    // auto newPos =  m_camera.getPosition() + delta;
    m_camera.setPosition(
        glm::vec3(
            -m_player.position + m_player.radius + glm::vec2(ctx.m_framebufferWidth/2.0f, ctx.m_framebufferHeight/2.0f)
            ,0.0f));
}

void GameView::onDraw(double time_since_start, float dt) {
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

        // {
        //     // Cubemap
        //     vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineSkybox.pipeline);
        //     m_descriptorSetCubemap.bind(cb, m_pipelineSkybox.layout);

        //     vkCmdBindVertexBuffers(cb, 0, 1, &m_cubemapBufferVertex.buffer, &_vOffset);
        //     m_uniformBuffer.pushConstant(m_pipelineSkybox.layout, VkShaderStageFlagBits(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT));

        //     const VkDeviceSize vertexCount{6*6};
        //     vkCmdDraw(cb, vertexCount, 1, 0, 0);
        // }
        vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.pipeline);
        m_descriptorSet.bind(cb, m_pipeline.layout);
        m_uniformBuffer.pushConstant(m_pipeline.layout, VkShaderStageFlagBits(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT));

        {
            glm::mat4 model{1.0f};
            vkCmdPushConstants(
                cb, m_pipeline.layout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                16, sizeof(glm::mat4), &model);


            for (int i = 0; i < (int)List::size(m_quadMesh); i++) {
                QuadMesh::Mesh cm = *List::get(m_quadMesh, i);
                // quad image

                vkCmdBindVertexBuffers(cb, 0, 1, &cm.m_bufferVertex.buffer, &_vOffset);
                vkCmdBindIndexBuffer(cb, cm.m_bufferIndices.buffer, 0, VK_INDEX_TYPE_UINT16);

                const VkDeviceSize indexCount{6};
                vkCmdDrawIndexed(cb, indexCount, 1, 0, 0, 0);
            }
        }

        // player
        {
            // vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.pipeline);
            // m_descriptorSet.bind(cb, m_pipeline.layout);

            vkCmdBindVertexBuffers(cb, 0, 1, &m_player.mesh.m_bufferVertex.buffer, &_vOffset);
            vkCmdBindIndexBuffer(cb, m_player.mesh.m_bufferIndices.buffer, 0, VK_INDEX_TYPE_UINT16);

            // m_uniformBuffer.pushConstant(m_pipeline.layout, VkShaderStageFlagBits(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT));

            // logD("{}", m_player.position);
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_player.position - m_player.radius, 0));
            vkCmdPushConstants(
                cb, m_pipeline.layout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                16, sizeof(glm::mat4), &model);


            const VkDeviceSize indexCount{6};
            vkCmdDrawIndexed(cb, indexCount, 1, 0, 0, 0);
        }
    });

    DebugDraw::instance().drawCube({0, 0, 0});
    DebugDraw::instance().drawAndFlush(cb, m_shaderData.projection * m_shaderData.view);

    ImGui::ShowDemoWindow();
}

void GameView::onKeyPress(int key) {
    auto& ctx = Context::instance();

    if (key == GLFW_KEY_C && !ImGui::GetIO().WantCaptureKeyboard) {
        ctx.toggleCursor();
    }
}

void GameView::onMouseMotion(int x, int y, int dx, int dy) {
}

void GameView::onResize(int width, int height) {
    m_camera.m_right = width;
    m_camera.m_top = height;
    m_camera.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}
