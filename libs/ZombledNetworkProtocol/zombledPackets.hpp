#pragma once

// Client bound (packets sent to client)

#include <cstdint>

namespace Zombled::Packets {
    namespace Server {

        struct Identification {
            uint8_t id;
            uint32_t entityId;
        };

        struct EntityAdd {
            uint8_t id;
            uint32_t entityId;
            uint32_t entityType;
            float x;
            float y;
            float z;
            float rot;
            uint8_t name[64];
        };

        struct EntityRemove {
            uint8_t id;
            uint32_t entityId;
        };

        struct EntityMove {
            uint8_t id;
            uint32_t entityId;
            float x;
            float y;
            float z;
            float rot;
        };
    };

    namespace Client {

        struct EntityMove {
            uint8_t id;
            float x;
            float y;
            float z;
            float rot;
        };

    };
};


// Server bound (packets sent to server)
