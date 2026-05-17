#include "uhcnet.hh"
#include "uhcio.hh"
#include "uhcstd.hh"

void* clientThread(void* arg) {
    TcpClient::It client = *(TcpClient::It*)arg;

    while (client.sockfd) {
        // Read
    }

    return NULL;
}

void serverAccept(TcpClient::It* client) {
    Log::info("New client joined");
    
    Thread::It thread = Thread::start(clientThread, client);
    Thread::join(thread);
    
    Log::info("Client left");
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
