#pragma once

#include <glm/ext/matrix_float4x4.hpp>

struct ShaderData {
    glm::mat4 projection{1.0f};
    glm::mat4 view{1.0f};
    glm::vec4 viewPosition{0.0f};
    glm::vec4 viewDirection{0.0f};
    float time{0.0f};
};
