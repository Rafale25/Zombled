#pragma once

#include <cstdint>
#include <unordered_map>

namespace Zombled::Packets {
    namespace Server {

#pragma pack(push, 1)

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

#pragma pack(pop)

        enum class PacketId {
            IDENTIFICATION = 0x00,
            ENTITY_ADD = 0x01,
            ENTITY_REMOVE = 0x02,
            ENTITY_MOVE = 0x03,
        };

        inline const std::unordered_map<PacketId, uint32_t> packetsSize = {
            { PacketId::IDENTIFICATION, sizeof(Identification) },
            { PacketId::ENTITY_ADD,     sizeof(EntityAdd)      },
            { PacketId::ENTITY_REMOVE,  sizeof(EntityRemove)   },
            { PacketId::ENTITY_MOVE,    sizeof(EntityMove)     },
        };
    };

    namespace Client {

#pragma pack(push, 1)

        struct EntityMove {
            uint8_t id;
            float x;
            float y;
            float z;
            float rot;
        };

#pragma pack(pop)

        enum class ClientPacketId {
            ENTITY_MOVE = 0x00,
        };

        inline const std::unordered_map<ClientPacketId, uint32_t> packetsSize = {
            { ClientPacketId::ENTITY_MOVE, sizeof(EntityMove) },
        };

    };

};
