#include <iostream>

#include "header.h"

int getClientAccess(int *serverSocket, int *clientSocket){
    sockaddr_in serverAddr{}, clientAddr{};
    socklen_t addrLen = sizeof(clientAddr);

    *serverSocket = (int) socket(AF_INET, SOCK_STREAM, 0);
    if (*serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12345);

    if (bind(*serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        close(*serverSocket);
        return -1;
    }

    if (listen(*serverSocket, 2) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        close(*serverSocket);
        return -1;
    }

    std::cout << "Waiting for player 1..." << std::endl;
    *clientSocket = accept(*serverSocket, (struct sockaddr *)&clientAddr, &addrLen);

    if (*clientSocket == -1) {
        std::cerr << "Error accepting connection for player 1\n";
        close(*serverSocket);
        return -1;
    }

    std::cout << "Player 'X' connected" << std::endl;
    return 0;
}


