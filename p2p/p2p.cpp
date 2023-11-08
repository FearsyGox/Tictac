#include "include/server.h" // startServer()
#include "include/client.h" // startClient()
#include "include/utils.h"  // detectMode()
#include "include/tictac.h" // tictac game funcitons
#include <iostream>

const int SIZE = 10;

using namespace std;

int clientStartGame(const int clientSocket);
int serverStartGame(const int server_clientSocket);

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
        clientStartGame(clientSocket);
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
        serverStartGame(server_clientSocket);
        closeServer(serverSocket, server_clientSocket);
    }
    return 0;
}


// NOTE: the send message block of code and recieve message could be made into its own function
int clientStartGame(const int clientSocket)
{
    
    bool runGame = true;
    int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    char response[SIZE];
    char message[SIZE];

    cout << endl
         << "Game started: type 'done' to quit" << endl;

    // start game
    while (runGame)
    {
        // send move to server
        printtictac(board);
        runGame = clientSendMessage(clientSocket, message);
        if (!runGame)
            break;

        // recieve move from server
        runGame = clientGetResponse(clientSocket, response);
        if(!runGame)
            break;
        cout << "\t" << response << endl;
    }

    return 0;
}


int serverStartGame(int server_clientSocket)
{
    bool runGame = true;
    int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    char message[SIZE];
    char response[SIZE];

    cout << endl
         << "Game started: type 'done' to quit.." << endl
         << endl;

    // start game
    while (runGame)
    {
        printtictac(board);
        // wait for client to make move
        cout << "Waiting for client to make a move..." << endl;

        // recieve move from client
        runGame = serverGetResponse(server_clientSocket, response);
        if (!runGame)
            break;

        // update the board
        cout << "\tClient Move: " << response << endl;
        // (processMove(board, atoi(response));
        printtictac(board);

        runGame = serverSendMessage(server_clientSocket, message);
        if (!runGame)
            break;
    }

    return 0;
}