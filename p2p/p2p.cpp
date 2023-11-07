#include "include/server.h" // startServer()
#include "include/client.h" // startClient()
#include "include/utils.h"  // detectMode()
#include <iostream>

using namespace std;
int startGame(bool isClient);

int main(int argc, char *argv[])
{
    bool isClient = detectMode(argc, argv);
    char *serverIp = argv[1];
    int serverPort = 12345;

    if (isClient)
    {
        int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        startClient(clientSocket, serverIp, serverPort);
        startGame(clientSocket, isClient);
        closeClient(clientSocket);
    }
    else // server
    {
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        int server_clientSocket;
        startServer(serverSocket, server_clientSocket, serverPort);
        startGame(serverSocket, isClient);
        closeServer(serverSocket, server_clientSocket);
    }
    return 0;
}

int startGame(int socket, bool isClient)
{
    cout << "Game started" << endl;

    if (isClient)
    {
     // Receive data from the server
    char buffer[1024];
    ssize_t bytesRead = recv(socket, buffer, sizeof(buffer), 0);
    if (bytesRead == -1) {
        std::cerr << "Error receiving data\n";
    } else {
        buffer[bytesRead] = '\0';  // Null-terminate the received data
        std::cout << "Received from server: " << buffer << "\n";
    }

    } else
    {
         // Send data to the client
    const char* message = "Hello, client!";
    send(server_clientSocket, message, strlen(message), 0);
    }


    return 0;
}


