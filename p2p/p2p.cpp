#include "include/server.h" // startServer()
#include "include/client.h" // startClient()
#include "include/utils.h"  // detectMode()
#include <fcntl.h>
#include <iostream>

using namespace std;

int startGameClient(int clientSocket);
int startGameServer(int server_clientSocket);

int main(int argc, char *argv[])
{
    bool isClient = detectMode(argc, argv);
    char *serverIp = argv[1];
    int serverPort = 12345;

    // server_clientSocket refers to the client's socket that is stored on the server
    int server_clientSocket = -1;

    // Client and Server follow different logic
    if (isClient)
    {
        // Create a socket
        int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1)
        {
            std::cerr << "Error creating socket\n";
            return -1;
        }

        startClient(clientSocket, serverIp, serverPort);
        startGameClient(clientSocket);
        closeClient(clientSocket);
    }
    else // server
    {
        // Create a socket
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1)
        {
            std::cerr << "Error creating socket\n";
            return -1;
        }

        startServer(serverSocket, &server_clientSocket, serverPort);
        cout << "server_clientSocket << " << server_clientSocket << endl;
        startGameServer(server_clientSocket);
        closeServer(serverSocket, server_clientSocket);
    }
    return 0;
}

int startGameClient(int clientSocket)
{
    cout << endl
         << "Game started" << endl;
    cout << "\ttype exit to quit" << endl;
    bool gameRunning = true;

    while (gameRunning)
    {
        char message[256];
        cout << "Enter a message" << endl;
        cin.getline(message, sizeof(message));

        if (send(clientSocket, message, strlen(message), 0) == -1)
        {
            cerr << "Error sending data\n";
            close(clientSocket);
            return -1;
        }

        if (strcmp(message, "exit") == 0)
        {
            break; // exit game
        }

        char response[256];
        int bytesRead = recv(clientSocket, response, sizeof(response), 0);
        if (bytesRead == -1)
        {
            cerr << "Error receiving response\n";
            // send(clientSocket, "close", 5, 0);
            close(clientSocket);
            return -1;
        }

        response[bytesRead] = '\0'; // Null-terminate the received data, DO WE NEED THIS????
        cout << response << endl;
    }

    return 0;
}

int startGameServer(int server_clientSocket)
{
    cout << endl
         << "Game started" << endl;

    
        char message[256];
        int bytesRead = recv(server_clientSocket, message, sizeof(message), 0);
        if (bytesRead == -1)
        {
            cerr << "Error receiving data\n";
            close(server_clientSocket);
            return -1;
        }

        message[bytesRead] = '\0'; // Null-terminate the received data
        cout << "Received from client: " << message << endl;

        char response[256] = "Server response: ";
        send(server_clientSocket, response, strlen(response), 0);

        if (strcmp(message, "exit") == 0)
        {
            close(server_clientSocket);
            // break; // exit game
        }
    

    return 0;
}
