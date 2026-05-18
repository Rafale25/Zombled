#include "uhcnet.hh"
#include "uhcio.hh"

namespace TcpServer {
#if defined(_WIN32) || defined(_WIN64)
    TcpServer::It create(short port, unsigned short slots) {
        TcpServer::It server;
        
        struct sockaddr_in serverAddress;
        WSADATA wsaData;

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            Log::warn("WSAStartup failed");
            return server;
        };

        server.sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (server.sockfd == INVALID_SOCKET) {
            Log::warn("Server socket creation failed");
            WSACleanup();
            return server;
        };

        memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);

        if (bind(server.sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
            Log::warn("Server bind failed");
            destroy(server);
            return server;
        };

        server.slots = slots;
        server.running = 1;
        return server;
    };

    void destroy(TcpServer::It& server) {
        closesocket(server.sockfd);
        WSACleanup();
        server.running = 0;
    };

    void listen(TcpServer::It& server, void (*callback)(TcpClient::It*)) {
        if (::listen(server.sockfd, server.slots) == SOCKET_ERROR) {
            Log::warn("Server listen failed");
            destroy(server);
            return;
        };

        while (server.running) {
            struct sockaddr_in clientAddress;
            socklen_t clientLength;
            TcpClient::It client;

            clientLength = sizeof(clientAddress);
            client.sockfd = accept(server.sockfd, (struct sockaddr *)&clientAddress, &clientLength);
            if (client.sockfd == INVALID_SOCKET) {
                Log::warn("Client accept failed");
                continue;
            };

            callback(&client);
        };
    };

    unsigned char write(TcpClient::It& client, const char* buffer, unsigned int size) {
        if (client.sockfd == INVALID_SOCKET) return 0;
        
        send(client.sockfd, (char*)buffer, size, 0);
        
        return 1;
    };

    void broadcast(List::It<TcpClient::It>& clients, char* buffer, unsigned int size) {
        unsigned long long clientSize = List::size(clients);

        for (unsigned long long i = 0; i < clientSize; i++) {
            TcpClient::It client = *List::get(clients, i);
            TcpServer::write(client, buffer, size);
        };
    };

    unsigned char read(TcpClient::It& client, char* buffer, unsigned int size) {
        if (client.sockfd == INVALID_SOCKET) return 0;

        int bytesRead = recv(client.sockfd, buffer, size, 0);
        if (bytesRead <= 0) return 0;

        return 1;
    };

    unsigned char readAll(TcpClient::It& client, char* buffer, unsigned int size) {
        if (client.sockfd == INVALID_SOCKET) return 0;

        int totalBytesRead = 0;
        while (totalBytesRead < size) {
            int bytesRead = recv(client.sockfd, buffer + totalBytesRead, size - totalBytesRead, 0);
            totalBytesRead += bytesRead;
            if (bytesRead <= 0) return 0;
        };

        return 1;
    };
#else
    TcpServer::It create(short port, unsigned short slots) {
        TcpServer::It server;
        
        struct sockaddr_in serverAddress;

        server.sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (server.sockfd < 0) {
            Log::warn("Server socket creation failed");
            return server;
        };

        memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);

        if (bind(server.sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
            Log::warn("Server bind failed");
            destroy(server);
            return server;
        };

        server.slots = slots;
        server.running = 1;
        return server;
    };

    void destroy(TcpServer::It& server) {
        ::close(server.sockfd);
        server.running = 0;
    };

    void listen(TcpServer::It& server, void (*callback)(TcpClient::It*)) {
        if (::listen(server.sockfd, server.slots) < 0) {
            Log::warn("Server listen failed");
            destroy(server);
            return;
        };

        while (server.running) {
            struct sockaddr_in clientAddress;
            socklen_t clientLength;
            TcpClient::It client;

            clientLength = sizeof(clientAddress);
            client.sockfd = accept(server.sockfd, (struct sockaddr *)&clientAddress, &clientLength);
            if (client.sockfd < 0) {
                Log::warn("Client accept failed");
                continue;
            };

            callback(&client);
        };
    };

    unsigned char write(TcpClient::It& client, const char* buffer, unsigned int size) {
        if (client.sockfd < 0) return 0;
        
        send(client.sockfd, buffer, size, MSG_NOSIGNAL);
        
        return 1;
    };

    void broadcast(List::It<TcpClient::It>& clients, char* buffer, unsigned int size) {
        unsigned long long clientSize = List::size(clients);

        for (unsigned long long i = 0; i < clientSize; i++) {
            TcpClient::It client = *List::get(clients, i);
            TcpServer::write(client, buffer, size);
        };
    };

    unsigned char read(TcpClient::It& client, char* buffer, unsigned int size) {
        if (client.sockfd < 0) return 0;

        int bytesRead = recv(client.sockfd, buffer, size, 0);
        if (bytesRead <= 0) return 0;

        return 1;
    };

    unsigned char readAll(TcpClient::It& client, char* buffer, unsigned int size) {
        if (client.sockfd < 0) return 0;

        int totalBytesRead = 0;
        while (totalBytesRead < size) {
            int bytesRead = recv(client.sockfd, buffer + totalBytesRead, size - totalBytesRead, 0);
            totalBytesRead += bytesRead;
            if (bytesRead <= 0) return 0;
        };

        return 1;
    };
#endif
};
