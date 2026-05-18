#pragma once

#include "uhcstd.hh"

#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <pthread.h>
    #include <unistd.h>
    #include <arpa/inet.h>
#endif

/// TCP client for connecting to a server
namespace TcpClient {
    /// Platform-specific TCP client handle
#if defined(_WIN32) || defined(_WIN64)
    struct It {
        SOCKET sockfd = INVALID_SOCKET;
        HANDLE thread = NULL;
    };
#else
    struct It {
        int sockfd = -1;
        pthread_t thread = NULL;
    };
#endif
    /// Create a TCP client and connect to a server
    /// @param ip Server IP address
    /// @param port Server port number
    /// @returns Connected client object
    TcpClient::It create(const char* ip, short port);
    /// Destroy a TCP client and close the connection
    /// @param client The TCP client
    void destroy(TcpClient::It& client);
    /// Send data to the server
    /// @param client The TCP client
    /// @param buffer Data to send
    /// @param size Number of bytes to send
    /// @returns Non-zero on success
    unsigned char write(TcpClient::It& client, const char* buffer, unsigned int size);
    /// Read data from the server (non-blocking, may be partial)
    /// @param client The TCP client
    /// @param buffer Receive buffer
    /// @param size Maximum bytes to read
    /// @returns Non-zero on success
    unsigned char read(TcpClient::It& client, char* buffer, unsigned int size);
    /// Read data from the server, blocking until all bytes arrive
    /// @param client The TCP client
    /// @param buffer Receive buffer
    /// @param size Exact number of bytes to read
    /// @returns Non-zero on success
    unsigned char readAll(TcpClient::It& client, char* buffer, unsigned int size);
};

/// TCP server for accepting and managing client connections
namespace TcpServer {
    /// Platform-specific TCP server handle
#if defined(_WIN32) || defined(_WIN64)
    struct It {
        SOCKET sockfd = INVALID_SOCKET;
        unsigned char running = 0;
        unsigned short slots = 0;
    };
#else
    struct It {
        int sockfd = -1;
        unsigned char running = 0;
        unsigned short slots = 0;
    };
#endif

    /// Create a TCP server bound to a port
    /// @param port Port to listen on
    /// @param slots Maximum simultaneous connections
    /// @returns Initialized server object
    TcpServer::It create(short port, unsigned short slots);
    /// Destroy a TCP server and close all connections
    /// @param server The TCP server
    void destroy(TcpServer::It& server);
    /// Start accepting connections, calling a callback for each new client
    /// @param server The TCP server
    /// @param callback Function called with each new client
    void listen(TcpServer::It& server, void (*callback)(TcpClient::It));
    /// Send data to a specific client
    /// @param client Target client
    /// @param buffer Data to send
    /// @param size Number of bytes to send
    /// @returns Non-zero on success
    unsigned char write(TcpClient::It& client, const char* buffer, unsigned int size);
    /// Read data from a client (non-blocking, may be partial)
    /// @param client Source client
    /// @param buffer Receive buffer
    /// @param size Maximum bytes to read
    /// @returns Non-zero on success
    unsigned char read(TcpClient::It& client, char* buffer, unsigned int size);
    /// Read data from a client, blocking until all bytes arrive
    /// @param client Source client
    /// @param buffer Receive buffer
    /// @param size Exact number of bytes to read
    /// @returns Non-zero on success
    unsigned char readAll(TcpClient::It& client, char* buffer, unsigned int size);
    /// Broadcast data to all clients in a list
    /// @param clients List of clients to send to
    /// @param buffer Data to broadcast
    /// @param size Number of bytes to send
    void broadcast(List::It<TcpClient::It>& clients, const char* buffer, unsigned int size);
};
