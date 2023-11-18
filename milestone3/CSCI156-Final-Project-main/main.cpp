#include "include/server.h" // startServer()
#include "include/client.h" // startClient()
#include "include/utils.h"  // detectMode()
#include "include/tictac.h" // tictac game funcitons
#include "unistd.h"
#include <thread>
#include <vector>
#include <iostream>

const int SIZE = 10;

using namespace std;

int clientStartGame(const int clientSocket);
int serverStartGame(const int welcomeSocket);
void displayRules(char letter);
void inserttictacSingle(int (&board)[9], int(&player));
void pvAI(int (&board)[9]);
// void handleClient(int welcomeSocket);

int main(int argc, char *argv[])
{
    bool isClient = detectMode(argc, argv);
    char *serverIp = argv[1];
    int serverPort = 12345;
    // int welcomeSocket;
    vector<int> clients;

    if (isClient)
    {
        int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1)
        {
            std::cerr << "Error creating socket\n";
            return -1;
        }
        startClient(clientSocket, serverIp, serverPort);
    }
    else
    { // server
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1)
        {
            std::cerr << "Error creating socket\n";
            return -1;
        }

        if (startServer(serverSocket, serverPort) == -1)
        {
            std::cerr << "Error starting server\n";
            return -1;
        }

        /*while (true)
        {
            welcomeSocket = accept(serverSocket, nullptr, nullptr);
            if (welcomeSocket == -1)
            {
                std::cerr << "Error accepting connection";
                continue;
            }

            cout << "Client Connection accepted" << endl;
            const char *initialMessage = "Select 1 to play versus a computer or 2 to play against another player";
            send(welcomeSocket, initialMessage, strlen(initialMessage), 0);
        }*/
    }
}

// update the game board, return true if game is over
bool updateGame(int *board, char move[SIZE], char letter)
{
    int position = move[0] - '0' - 1;
    using namespace std;
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

// client starts game, gets move from user, sends message, recieves response
// continue this loop until game is over or someone quits
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
        if (!clientSendMessage(clientSocket, message))
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
        cout << "Server Move: " << endl;

        // update game, check for end of game
        if (updateGame(board, response, 'O'))
        {
            // end of game
            break;
        }
    }

    return 0;
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

//

void inserttictacSingle(int (&board)[9], int(&player))
{

    // If the player is AI
    if (player == 2)
    {
        int position = rand() % (9);
        // Randomize the input until there is a valid input
        while (board[position] != 0)
        {
            position = rand() % (9);
        }

        // Place the input as the number the AI has choosen
        board[position] = 2;

        // Change the turn from AI to Player
        player = 1;
    }
    else
    {
        int position;
        // Give the player the infomation of what inputs the player can do
        cout << "Pick a location from 0 - 8" << endl;
        cin >> position;

        // If the Input is wrong or already existed then keep making the input until it is valid
        while (board[position] != 0)
        {
            printtictac(board);
            cout << "INVALID POSITION!!" << endl;
            cout << "Pick a location from 0 - 8" << endl;
            cin >> position;
        }
        // Place the input as the number the player is
        board[position] = 1;

        // Change the turn from player to AI
        player = 2;
    }
}

void pvAI(int (&board)[9])
{
    bool stopgame = false;
    int player = 1;

    while (!stopgame)
    {

        printtictac(board);             // update the user what is going on in the board
        stopgame = checkendgame(board); // check if there are winners

        // cout << player << endl;

        // if there is no winner then give input for the person's turn
        if (stopgame == false)
        {
            inserttictacSingle(board, player); // either it would be the player or AI Turn
        }
    }
}