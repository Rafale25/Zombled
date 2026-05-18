#pragma once

#include "uhcnet.hh"
#include "zombledPackets.hpp"

struct GameState {
    TcpClient::It networkClient{};

    // Entity
    // ...
};

inline GameState g_gameState{};
