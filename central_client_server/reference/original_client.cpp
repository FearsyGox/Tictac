// client

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    // Connect to the server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);  // Port number
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP address

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to server\n";
        close(clientSocket);
        return -1;
    }

    std::cout << "Connected to server\n";

    // Receive data from the server
    char buffer[1024];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1) {
        std::cerr << "Error receiving data\n";
    } else {
        buffer[bytesRead] = '\0';  // Null-terminate the received data
        std::cout << "Received from server: " << buffer << "\n";
    }

    // Close the socket
    close(clientSocket);

    return 0;
}