#pragma once

#include "uhcnet.hh"
#include "zombledPackets.hpp"

struct GameState {
    TcpClient::It networkClient;
    Thread::It networkThread;
};

inline GameState g_gameState{};
