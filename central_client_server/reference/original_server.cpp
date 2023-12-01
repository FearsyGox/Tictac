// server.cpp

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    // Bind the socket to an address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);  // Port number
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
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientSize);
    if (clientSocket == -1) {
        std::cerr << "Error accepting connection\n";
        close(serverSocket);
        return -1;
    }

    std::cout << "Connection accepted from " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << "\n";

    // Send data to the client
    const char* message = "Hello, client!";
    send(clientSocket, message, strlen(message), 0);

    // Close the sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}