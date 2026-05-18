#include "uhcnet.hh"
#include "uhcio.hh"
#include "uhcstd.hh"
#include "zombledPackets.hpp"

static uint32_t entityId = 0;
static List::It<TcpClient::It> clients;

void* clientThread(void* arg) {
    TcpClient::It client = *(TcpClient::It*)arg;

    uint32_t clientEntityId = entityId;

    Log::debug("before");

    UhcBuffer::It buffer2;
    buffer2.handle = malloc(1024);
    UhcBuffer::setOrder(buffer2, UHC_BIG_ENDIAN);
    UhcBuffer::reset(buffer2);

    UhcBuffer::It buffer;
    buffer.handle = malloc(1024);
    UhcBuffer::setOrder(buffer, UHC_BIG_ENDIAN);
    UhcBuffer::reset(buffer);
    UhcBuffer::putU8(buffer, 0x00);
    UhcBuffer::putU32(buffer, entityId);

    Log::debug("after malloc");

    TcpServer::write(client, (const char*)buffer.handle, 5);

    Log::debug("after write");

    // broadcast to every client
    UhcBuffer::reset(buffer);
    UhcBuffer::putU8(buffer, 0x01);
    UhcBuffer::putU32(buffer, entityId);
    UhcBuffer::putU32(buffer, 0);
    UhcBuffer::putF32(buffer, 0);
    UhcBuffer::putF32(buffer, 0);
    UhcBuffer::putF32(buffer, 0);
    UhcBuffer::putF32(buffer, 0);

    using namespace Zombled::Packets;

    uint8_t size = Server::packetsSize.at(Server::PacketId::ENTITY_ADD);
    TcpServer::broadcast(clients, (const char*)buffer.handle, size);
    List::add(clients, client);

    entityId++;

    while (client.sockfd) {
        TcpServer::readAll(client, (char*)buffer.handle, 1);

        uint8_t value = UhcBuffer::getU8(buffer);
        Server::PacketId packetId = (Server::PacketId)value;

        uint8_t size = Server::packetsSize.at(packetId);
        TcpServer::readAll(client, (char*)(buffer.handle) + 1, size - 1);


        Client::EntityMove clientPacket;

        UhcBuffer::reset(buffer);
        clientPacket.id = UhcBuffer::getU8(buffer);
        clientPacket.x = UhcBuffer::getF32(buffer);
        clientPacket.y = UhcBuffer::getF32(buffer);
        clientPacket.z = UhcBuffer::getF32(buffer);
        clientPacket.rot = UhcBuffer::getF32(buffer);

        UhcBuffer::reset(buffer2);
        UhcBuffer::putU8(buffer2, 0x03);
        UhcBuffer::putU32(buffer2, clientEntityId);
        UhcBuffer::putF32(buffer2, clientPacket.x);
        UhcBuffer::putF32(buffer2, clientPacket.y);
        UhcBuffer::putF32(buffer2, clientPacket.z);
        UhcBuffer::putF32(buffer2, clientPacket.rot);

        TcpServer::broadcast(clients, (const char*)buffer2.handle, sizeof(Client::EntityMove));
    }

    Log::info("Client left");

    free(buffer.handle);

    return NULL;
}

void serverAccept(TcpClient::It client) {
    Log::info("New client joined");

    Thread::It thread = Thread::start(clientThread, &client);
}

void* serverListen(void* arg) {
    Log::info("Server listening");

    TcpServer::It server = *(TcpServer::It*)arg;

    while (server.running) {
        TcpServer::listen(server, serverAccept);
    }

    return NULL;
}

int main() {
    TcpServer::It server = TcpServer::create(22222, 10);

    Thread::It thread = Thread::start(serverListen, &server);

    while (server.running) {
        // Send
    }

    Thread::join(thread);

    TcpServer::destroy(server);

    return 0;
}
