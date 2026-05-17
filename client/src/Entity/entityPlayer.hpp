#pragma once

#include <cmath>
#include <optional>
#include <utility>

#include "entity.hpp"
#include "Mesh/quadMesh.hpp"
#include "MapLoader/mapLoader.hpp"

class EntityPlayer : public Entity {
public:
    QuadMesh::Mesh mesh;
    glm::vec2 velocity;
    float radius = 16.0f;
    float speed = 200.0f;

    explicit EntityPlayer(
        glm::vec2 initialPosition = glm::vec2(),
        glm::vec2 initialSize = glm::vec2(32.0f, 32.0f)
    ): Entity(initialPosition, initialSize)
    {
        mesh = QuadMesh::create(initialPosition.x, initialPosition.y, initialSize.x, initialSize.y, 1);
    }

    void update(float dt, MapLoader::Map& map) override;
    void render() override;

private:
    static constexpr int COLLISION_LAYER = 0;

    void moveAndCollide(float dt, MapLoader::Map& map);
    void resolveCollisions(MapLoader::Map& map);
    std::optional<std::pair<float, glm::vec2>> circleVsAabb(const glm::vec2& circleCenter, float circleRadius, const glm::vec2& rectPos, const glm::vec2& rectSize);
};
