// server.cpp

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>

using namespace std;

void handleClient(int welcomeSocket);

// welcomeSocket refers to the client's socket that is stored on the server
// this is different from the clientSocket in client.h, this naming convention is used to avoid conflict in p2p.cpp

int startServer(int serverSocket, int port)
{

    // Bind the socket to an address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);       // Port number
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Accept connections from any address

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "Error binding socket\n";
        close(serverSocket);
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1)
    {
        std::cerr << "Error listening for connections\n";
        close(serverSocket);
        return -1;
    }

    std::cout << "Server listening on port 12345...\n";

    while (true)
    {
        // Accept a connection
        sockaddr_in clientAddress;
        socklen_t clientSize = sizeof(clientAddress);
        int welcomeSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientSize);

        if (welcomeSocket == -1)
        {
            std::cerr << "Error accepting connection\n";
            close(serverSocket);
            close(welcomeSocket);
            return -1;
        }

        std::cout << "\tConnection accepted from " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << "\n";

        vector<thread> clients;
        clients.emplace_back(handleClient, welcomeSocket);
        clients.back().detach();
    }
}

int closeServer(int serverSocket, int welcomeSocket)
{
    // Close the sockets
    close(welcomeSocket);
    close(serverSocket);
    return 0;
}

bool serverGetResponse(int welcomeSocket, char *response)
{
    // recieve message
    int bytesRead = recv(welcomeSocket, response, sizeof(response), 0);
    response[bytesRead] = '\0'; // Null-terminate the received data

    if (bytesRead == -1)
    {
        cerr << "Error receiving data\n";
        send(welcomeSocket, "done", 4, 0); // tell client to exit
        close(welcomeSocket);
        return false;
    }

    if (strcmp(response, "done") == 0)
    {
        cout << "Other player quit game, exiting..." << endl;
        return false; // exit game
    }

    return true;
}

bool serverSendMessage(int welcomeSocket, char *message)
{
    // send message
    send(welcomeSocket, message, strlen(message), 0);

    if (strcmp(message, "done") == 0)
    {
        // DOES SERVER_CLIENT SOCKET NEED TO BE CLOSED HERE???????
        cout << "Exiting..." << endl;
        return false;
    }

    return true;
}

void handleClient(int welcomeSocket)
{
    const char *initialMessage = "Press 1 to play against a computer or 2 to play against another player!"; // Customize this message
    int sentBytes = send(welcomeSocket, initialMessage, strlen(initialMessage), 0);

    if (sentBytes == -1)
    {
        std::cerr << "Error sending welcome message to the client\n";
        close(welcomeSocket);
        return;
    }

    // Loop for handling client requests
    // while (true)
    //{
    // ...
    //}

    std::cout << "Sent initial message to the client\n"; // used to debug - message isn't received by client

    close(welcomeSocket);
}

#endif
