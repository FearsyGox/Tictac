// server.cpp

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

// server_clientSocket refers to the client's socket that is stored on the server
// this is different from the clientSocket in client.h, this naming convention is used to avoid conflict in p2p.cpp

int startServer(int serverSocket, int *server_clientSocket, int port) {

    // Bind the socket to an address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);  // Port number
    serverAddress.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any address

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket\n";
        close(serverSocket);
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening for connections\n";
        close(serverSocket);
        return -1;
    }

    std::cout << "Server listening on port 12345...\n";

    // Accept a connection
    sockaddr_in clientAddress;
    socklen_t clientSize = sizeof(clientAddress);
    *server_clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientSize);

    if (*server_clientSocket == -1) {
        std::cerr << "Error accepting connection\n";
        close(serverSocket);
        close(*server_clientSocket);
        return -1;
    }

    std::cout << "\tConnection accepted from " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << "\n";

    return 0;
}

int closeServer(int serverSocket, int server_clientSocket) {
    // Close the sockets
    close(server_clientSocket);
    close(serverSocket);
    return 0;
}


#endif