#include <vulkan/vulkan.h>

#include "entityPlayer.hpp"
#include "uhcstd.hh"

void EntityPlayer::update(float dt, MapLoader::Map& map) {
    moveAndCollide(dt, map);
    velocity = glm::vec2(0.0f);

    //playerSprite.x = position.x - radius;
    //playerSprite.y = position.y - radius;

    //Camera::position.x = -position.x + (Window::width  / 2.f);
    //Camera::position.y = -position.y + (Window::height / 2.f);
    //Camera::view.setIdentity().translate(Camera::position.x, Camera::position.y, Camera::position.z);
}

void EntityPlayer::render() {
    //TODO: :(
}

void EntityPlayer::moveAndCollide(float dt, MapLoader::Map& map) {
    position.x += velocity.x * dt;
    resolveCollisions(map);

    position.y += velocity.y * dt;
    resolveCollisions(map);
}

void EntityPlayer::resolveCollisions(MapLoader::Map& map) {
    MapLoader::MapLayer collisionLayer = *List::first(map.layers);

    for (int i = 0; i < (int)List::size(collisionLayer.tiles); i++) {
        MapLoader::MapTile tile = *List::get(collisionLayer.tiles, i);
        auto result = circleVsAabb(position, radius, {tile.x, tile.y}, {tile.width, tile.height});
        if (!result) continue;

        auto [penetration, normal] = *result;

        position.x += normal.x * penetration;
        position.y += normal.y * penetration;

        float dot = velocity.x * normal.x + velocity.y * normal.y;
        if (dot < 0.f) {
            velocity.x -= dot * normal.x;
            velocity.y -= dot * normal.y;
        }
    }
}

std::optional<std::pair<float, glm::vec2>> EntityPlayer::circleVsAabb(const glm::vec2& circleCenter, float circleRadius, const glm::vec2& rectPos, const glm::vec2& rectSize) {
    const float rectMinX = rectPos.x;
    const float rectMinY = rectPos.y;
    const float rectMaxX = rectPos.x + rectSize.x;
    const float rectMaxY = rectPos.y + rectSize.y;

    const float closestX = glm::clamp(circleCenter.x, rectMinX, rectMaxX);
    const float closestY = glm::clamp(circleCenter.y, rectMinY, rectMaxY);

    const float dx = circleCenter.x - closestX;
    const float dy = circleCenter.y - closestY;
    const float distSq = dx * dx + dy * dy;

    if (distSq > 0.0f) {
        if (distSq >= circleRadius * circleRadius) return std::nullopt;

        const float dist = glm::sqrt(distSq);
        const glm::vec2 normal = glm::vec2(dx / dist, dy / dist);
        const float penetration = circleRadius - dist;
        return std::make_pair(penetration, normal);
    }

    const float overlapLeft = circleCenter.x - rectMinX;
    const float overlapRight = rectMaxX - circleCenter.x;
    const float overlapTop = circleCenter.y - rectMinY;
    const float overlapBottom = rectMaxY - circleCenter.y;

    const float minOverlap = std::min({ overlapLeft, overlapRight,
                                        overlapTop, overlapBottom });

    glm::vec2 normal;
    if (minOverlap == overlapLeft) normal = glm::vec2(-1.f, 0.f);
    else if (minOverlap == overlapRight) normal = glm::vec2(1.f, 0.f);
    else if (minOverlap == overlapTop) normal = glm::vec2(0.f, -1.f);
    else normal = glm::vec2(0.f, 1.f);

    const float penetration = circleRadius + minOverlap;
    return std::make_pair(penetration, normal);
}
