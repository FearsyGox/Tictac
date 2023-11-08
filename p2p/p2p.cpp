#include "include/server.h" // startServer()
#include "include/client.h" // startClient()
#include "include/utils.h"  // detectMode()
#include "include/tictac.h" // tictac game funcitons
#include <iostream>

const int SIZE = 10;

using namespace std;

int clientStartGame(const int clientSocket);
int serverStartGame(const int server_clientSocket);
void displayRules(char letter);

int main(int argc, char *argv[])
{
    bool isClient = detectMode(argc, argv);
    char *serverIp = argv[1];
    int serverPort = 12345;

    // server_clientSocket refers to the client's socket that is stored on the server

    // Client and Server follow different logic
    if (isClient)
    {
        // Create client socket
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
        // Create server's client socket
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

// ADD LOGIC HERE TO KEEP GAME GOING IF INVALID MOVE IS MADE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
bool updateGameState(int *board, char move[SIZE], char letter)
{
    int position = move[0] - '0' - 1;
}

// NOTE: the send message block of code and recieve message could be made into its own function
int clientStartGame(const int clientSocket)
{
    bool runGame = true;
    int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    char letter = 'X';
    char response[SIZE];
    char message[SIZE];

    displayRules(letter);

    // start game
    while (runGame)
    {
        // display board
        printtictac(board);
        cout << endl;

        // get move and send message
        getMove(board, response, SIZE);
        if(!clientSendMessage(clientSocket, message, SIZE))
            break;
        
        // update move
        updateGameState(board, message, letter);
        printtictac(board);
        cout << endl;

        // recieive response
        if(!clientGetResponse(clientSocket, response))
            break;
        cout << "Server Move: " << response << endl;

        // update game state
        updateGameState(board, response, letter);
    }

    return 0;
}

int serverStartGame(int server_clientSocket)
{
    bool runGame = true;
    int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    char letter = 'O';
    char message[SIZE];
    char response[SIZE];

    displayRules('O');

    // start game
    while (runGame)
    {
        // display board
        printtictac(board);
        cout << endl;

        // recieve response
        cout << "Waiting for client to make a move..." << endl;
        if (!serverGetResponse(server_clientSocket, response))
            break;
        cout << "Client Move: " << response << endl;

        // update game state
        updateGameState(board, response, letter);
        printtictac(board);
        cout << endl;

        // get move and send message
        getMove(board, message, SIZE);
        if (!serverSendMessage(server_clientSocket, message, SIZE))
            break;
        updateGameState(board, message, letter);
    }

    return 0;
}



