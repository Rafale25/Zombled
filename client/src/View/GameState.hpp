#pragma once

#include "uhcnet.hh"
#include "zombledPackets.hpp"
#include "Entity/entity.hpp"
#include <unordered_map>
#include <memory>

struct GameState {
    TcpClient::It networkClient;
    Thread::It networkThread;

    // <entityId, Entity>
    std::unordered_map<uint32_t, std::unique_ptr<Entity>> entities;
};

inline GameState g_gameState{};
