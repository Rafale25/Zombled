#pragma once

#include <glm/glm.hpp>

#include "MapLoader/mapLoader.hpp"

class Entity {
public:
    glm::vec2 position{};
    glm::vec2 size{};
    float rot{};
    uint32_t id{};
    uint8_t name[64]{};

    explicit Entity(glm::vec2 position = glm::vec2(), glm::vec2 size = glm::vec2()): position(position), size(size)
    {}

    virtual ~Entity() = default;

    virtual void update(float dt, MapLoader::Map& map) = 0;
    virtual void render() = 0;
};
