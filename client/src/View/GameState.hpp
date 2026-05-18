#pragma once

#include "uhcnet.hh"

struct GameState {
    TcpClient::It networkClient{};
    // ...
};

inline GameState g_gameState{};
