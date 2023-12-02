#include "include/server.h" // startServer()
#include "include/client.h" // startClient()
#include "include/utils.h"  // detectMode()
#include "include/tictac.h" // tictac game funcitons
#include <iostream>

const int SIZE = 10;

using namespace std;

int clientStartGame(const int clientSocket);
int serverStartGame(const int welcomeSocket);
void displayRules(char letter);

int main(int argc, char *argv[])
{
        int serverPort = 12345;

        // Create server's client socket
        int welcomeSocket = -1;
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1)
        {
            std::cerr << "Error creating socket\n";
            return -1;
        }

        if (startServer(serverSocket, &welcomeSocket, serverPort) == -1)
        {
            cout << "\tserver_clientSocket << " << welcomeSocket << endl;
            return -1;
        }

        // create thread to handle client
        serverStartGame(welcomeSocket);
        closeServer(serverSocket, welcomeSocket);
    return 0;
}

// update the game board, return true if game is over
bool updateGame(int *board, char move[SIZE], char letter)
{
    int position = move[0] - '0' - 1;
    if (letter == 'X')
    {
        board[position] = 1;
    }
    if (letter == 'O')
    {
        board[position] = 2;
    }

    // game not over
    return false;

    // return checkendgame(board);
    // CHECK IF GAME IS OVER HERE!!!!!!!!!
    // PRINT RESULT IF GAME IS OVER!!!!!!!!
    // THEN RETURN FALSE, OUTER FUNCTION SHOULD HANDLE THE REST!!!!!!!!!!
}

// server starts game, recieves response, gets move, sends message
// continue this loop until game is over or someone quits
int serverStartGame(int welcome_socket)
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
        if (!serverGetResponse(welcome_socket, response))
            break;
        cout << "Client Move: " << endl;

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
        if (!serverSendMessage(welcome_socket, message))
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
