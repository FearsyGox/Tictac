// client.cpp

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

// ip and port refer to the server's ip and port
int startClient(int clientSocket, char *ip, int port) {
   
    // Connect to the server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);  // Port number
    serverAddress.sin_addr.s_addr = inet_addr(ip);  // Server IP address

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to server\n";
        close(clientSocket);
        return -1;
    }

    std::cout << "Connected to server\n";

    return 0;
}

int closeClient(int clientSocket) {
    // Close the socket
    close(clientSocket);
    return 0;
}

#endif