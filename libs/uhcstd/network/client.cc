#include "uhcnet.hh"
#include "uhcio.hh"

namespace TcpClient {
#if defined(_WIN32) || defined(_WIN64)
    TcpClient::It create(const char* ip, short port) {
        TcpClient::It client;

        struct sockaddr_in server_addr;

        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            Log::warn("WSA startup failed");
            return client;
        };

        client.sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (client.sockfd == INVALID_SOCKET) {
            Log::warn("Socket creation failed, error code: %d", WSAGetLastError());
            WSACleanup();
            return client;
        };

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family      = AF_INET;
        server_addr.sin_port        = htons(port);

        if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
            Log::warn("Invalid server address");
            destroy(client);
            return client;
        };

        if (connect(client.sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
            Log::warn("Connection failed");
            destroy(client);
            return client;
        };

        return client;
    };

    void destroy(TcpClient::It& client) {
        if (client.sockfd == INVALID_SOCKET) return;
        closesocket(client.sockfd);
        client.sockfd = INVALID_SOCKET;
        WSACleanup();
    };

    unsigned char write(TcpClient::It& client, const char* buffer, unsigned int size) {
        if (client.sockfd == INVALID_SOCKET) return 1;
        send(client.sockfd, buffer, size, 0);

        return 0;
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
    TcpClient::It create(const char* ip, short port) {
        TcpClient::It client;

        struct sockaddr_in server_addr;

        client.sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (client.sockfd < 0) {
            Log::warn("Socket creation failed");
            return client;
        };

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
            Log::warn("Invalid server address");
            destroy(client);
            return client;
        };

        if (connect(client.sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            Log::warn("Connection failed");
            destroy(client);
            return client;
        };

        return client;
    };

    void destroy(TcpClient::It& client) {
        if (client.sockfd < 0) return;
        close(client.sockfd);
        client.sockfd = -1;
    };

    unsigned char write(TcpClient::It& client, const char* buffer, unsigned int size) {
        if (client.sockfd < 0) return 0;

        send(client.sockfd, buffer, size, 0);

        return 1;
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
