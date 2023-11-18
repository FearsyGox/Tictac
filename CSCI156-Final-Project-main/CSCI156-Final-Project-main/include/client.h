// client.cpp

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>

using namespace std;

void clientThread(int clientSocket)
{
    // Your existing client handling logic...
    // For example, reading and writing to the server

    // Close the client socket when done
    close(clientSocket);
}

// ip and port refer to the server's ip and port
int startClient(int clientSocket, char *ip, int port)
{

    // Connect to the server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);          // Port number
    serverAddress.sin_addr.s_addr = inet_addr(ip); // Server IP address

    std::cout << "Attempting to connect to server...\n";

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "Error connecting to server\n";
        close(clientSocket);
        return -1;
    }

    std::cout << "Connected to server\n";
    return 0;
}

// client sends a message to the server
bool clientSendMessage(const int clientSocket, char message[])
{
    // send message
    if (send(clientSocket, message, strlen(message), 0) == -1)
    {
        cerr << "Error sending data\n";
        close(clientSocket);
        return false;
    }

    if (strcmp(message, "done") == 0)
    {
        cout << "Exiting..." << endl;
        return false;
    }

    return true;
}

// client recieves a response from the server
bool clientGetResponse(const int clientSocket, char *response)
{
    // recieve message
    int bytesRead = recv(clientSocket, response, sizeof(response), 0);
    response[bytesRead] = '\0'; // null-terminate response
    if (bytesRead == -1)
    {
        cerr << "Error receiving response\n";
        send(clientSocket, "done", 4, 0); // tell server to exit
        close(clientSocket);
        return false;
    }

    /*if (strcmp(response, "You chose to play against a computer") == 0)
    {
        cout << response << endl;
        return true;
    }*/

    cout << "Received message: " << response << endl;

    if (strcmp(response, "done") == 0)
    {
        cout << "Other player quit game, exiting..." << endl;
        return false;
    }
    return true;
}

int closeClient(int clientSocket)
{
    // Close the socket
    close(clientSocket);
    return 0;
}

#endif