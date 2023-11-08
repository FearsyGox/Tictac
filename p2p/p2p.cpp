#include "include/server.h" // startServer()
#include "include/client.h" // startClient()
#include "include/utils.h"  // detectMode()
#include "include/tictac.h" // tictac game funcitons
#include <fcntl.h>
#include <iostream>

const int SIZE = 10;


using namespace std;

int startGameClient(const int clientSocket);
int startGameServer(const int server_clientSocket);

int main(int argc, char *argv[])
{
    bool isClient = detectMode(argc, argv);
    char *serverIp = argv[1];
    int serverPort = 12345;

    // server_clientSocket refers to the client's socket that is stored on the server

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
        int server_clientSocket = -1;
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1)
        {
            std::cerr << "Error creating socket\n";
            return -1;
        }

        startServer(serverSocket, &server_clientSocket, serverPort);
        cout << "\tserver_clientSocket << " << server_clientSocket << endl;
        startGameServer(server_clientSocket);
        closeServer(serverSocket, server_clientSocket);
    }
    return 0;
}

void clientSendMessage(const int clientSocket, char *message)
{
     // send message
        cout << "Enter move: ";
        cin.getline(message, sizeof(message));

        //memset(message, 0, SIZE);
        if (send(clientSocket, message, strlen(message), 0) == -1)
        {
            cerr << "Error sending data\n";
            close(clientSocket);
        }
}

void clientGetResponse(const int clientSocket, char *response)
{
    // recieve message
        int bytesRead = recv(clientSocket, response, sizeof(response), 0);
        response[bytesRead] = '\0'; // null-terminate response
        if (bytesRead == -1)
        {
            cerr << "Error receiving response\n";
            send(clientSocket, "done", 4, 0);       // tell server to exit
            close(clientSocket);
        }
}

// NOTE: the send message block of code and recieve message could be made into its own function
int startGameClient(const int clientSocket)
{
    cout << endl
         << "Game started: type 'done' to quit" << endl;
    bool gameRunning = true;
    int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    char message[SIZE];
    char response[SIZE];


    // start game
    while (gameRunning)
    {
        printtictac(board);
        clientSendMessage(clientSocket, message);

        // exit game
        if (strcmp(message, "done") == 0)
        {
            cout << "Exiting..." << endl;
            break;
        }

        clientGetResponse(clientSocket, response);

        // exit game on servers request
        if (strcmp(response, "done") == 0)
        {
            cout << "Other player quit game, exiting..." << endl;
            break; // exit game
        }

        cout << "\t" << response << endl;
    }

    return 0;
}

int startGameServer(int server_clientSocket)
{
    bool gameRunning = true;
    int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    char response[SIZE];
    char message[SIZE];

    cout << endl
         << "Game started: type 'done' to quit.." << endl;
    cout << "Waiting for client to make a move..." << endl
         << endl;

    while (gameRunning)
    {

        // recieve message
        //memset(message, 0, SIZE);
        int bytesRead = recv(server_clientSocket, response, sizeof(response), 0);
        response[bytesRead] = '\0'; // Null-terminate the received data

        if (bytesRead == -1)
        {
            cerr << "Error receiving data\n";
            send(server_clientSocket, "done", 4, 0); // tell client to exit
            close(server_clientSocket);
            return -1;
        }

        // exit game on clients request
        if (strcmp(response, "done") == 0)
        {
            cout << "Other player quit game, exiting..." << endl;
            break; // exit game
        }

        cout << "\t" << response << endl;

        // send message
        cout << "Enter move: ";
       // memset(response, 0, SIZE);
        cin.getline(message, sizeof(message));
        message[bytesRead] = '\0'; // null-terminate response

        send(server_clientSocket, message, strlen(message), 0);

        // exit game on servers request
        if (strcmp(message, "done") == 0)
        {
            cout << "Exiting..." << endl;
            break;
        }
    }

    return 0;
}

int gameState(char *state)
{
    return 0;
}