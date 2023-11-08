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

        if (startServer(serverSocket, &server_clientSocket, serverPort) == -1)
        {
            cout << "\tserver_clientSocket << " << server_clientSocket << endl;
            return -1;
        }
        serverStartGame(server_clientSocket);
        closeServer(serverSocket, server_clientSocket);
    }
    return 0;
}

// update the game board, return true if game is over
bool updateGame(int *board, char move[SIZE], char letter)
{
    int position = move[0] - '0' - 1;
    cout << "position: " << position << endl;
    cout << "letter: " << letter << endl;
    if (letter == 'X')
    {
        board[position] = 1;
        cout << "board[position]: " << board[position] << endl;
    }
    if (letter == 'O')
    {
        board[position] = 2;
        cout << "board[position]: " << board[position] << endl;
    }
    
    // game not over
    return false;
    
    // CHECK IF GAME IS OVER HERE!!!!!!!!!
    // PRINT RESULT IF GAME IS OVER!!!!!!!!
    // THEN RETURN FALSE, OUTER FUNCTION SHOULD HANDLE THE REST!!!!!!!!!!

}

void printBoard(int board[SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        cout << board[i] << " ";
        if ((i + 1) % 3 == 0)
        {
            cout << endl;
        }
    }
}

// NOTE: the send message block of code and recieve message could be made into its own function
int clientStartGame(const int clientSocket)
{
    bool endOfGame = false;
    int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    char letter = 'X';
    char response[SIZE];
    char message[SIZE];

    displayRules(letter);

    // start game
    while (!endOfGame)
    {
        // display board
        printtictac(board);
        cout << endl;

        // get move, send message
        getMove(board, message, SIZE);
        if (!clientSendMessage(clientSocket, message, SIZE))
        {
            // either client quit or error occured
            break;
        }

        // update game, check for end of game
        if (updateGame(board, message, 'X'))
        {
            // end of game
            break;
        }

        // display board
        printtictac(board);
        cout << endl;

        // recieive response
        cout << "Waiting for server to make a move..." << endl;
        if (!clientGetResponse(clientSocket, response))
            break;
        cout << "Server Move: " << response << endl;

        // update game, check for end of game
        if (updateGame(board, response, 'O'))
        {
            // end of game
            break;
        }
    }

    return 0;
}

int serverStartGame(int server_clientSocket)
{
    bool endOfGame = false;
    int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    char letter = 'O';
    char message[SIZE];
    char response[SIZE];

    displayRules('O');

    // start game
    while (!endOfGame)
    {
        // display board
        printtictac(board);
        cout << endl;

        // recieve response
        cout << "Waiting for client to make a move..." << endl;
        if (!serverGetResponse(server_clientSocket, response))
            break;
        cout << "Client Move: " << response << endl;
        
        // update game, check for end of game
        if (updateGame(board, response, 'X'))
        {
            // end of game
            break;
        }

        // display board
        printtictac(board);
        cout << endl;

        // get move, send message
        getMove(board, message, SIZE);
        if (!serverSendMessage(server_clientSocket, message, SIZE))
            break;

        // update game and check for end of game
        if (updateGame(board, message, 'O'))
        {
            // end of game
            break;
        }
    }

    return 0;
}
