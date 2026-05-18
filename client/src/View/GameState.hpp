#pragma once

#include "uhcnet.hh"
#include "zombledPackets.hpp"
#include "Entity/entity.hpp"

struct GameState {
    TcpClient::It networkClient;
    Thread::It networkThread;

    // <entityId, Entity>
    std::unordered_map<uint32_t, Entity> entities;
};

inline GameState g_gameState{};
