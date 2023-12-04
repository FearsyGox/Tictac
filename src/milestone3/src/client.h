#include <iostream>
#include "game.h"

#include "header.h"

int connectToServer(const char *server_ip_addr, int port)
{

    int clientSocket = (int)socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(server_ip_addr);
    serverAddr.sin_port = htons(port);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Error connecting to server\n";
        close(clientSocket);
        return -1;
    }

    std::cout << "Connected to server\n";

    return clientSocket;
}

void makeMove(int clientSocket, char board[3][3])
{
    int row, col;

    std::cout << "Enter your move (row and column): ";
    std::cout << "Enter your move row[1-3] and column[1-3]: ";
    std::cin >> row >> col;
    row--;
    col--;

    while (row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != ' ')
    {
        std::cout << "Invalid move. Try again." << std::endl;
        std::cout << "Enter your move row[1-3] and column[1-3]: ";
        std::cin >> row >> col;
        row--;
        col--;
    }

    send(clientSocket, reinterpret_cast<const char *>(&row), sizeof(row), 0);
    send(clientSocket, reinterpret_cast<const char *>(&col), sizeof(col), 0);
}

void playerVsAI(int clientSocket)
{
    char currentPlayer = 'X';

        while (true)
        {

            char board[3][3];
            char state;

            recv(clientSocket, &board, sizeof(board), 0);
            printBoard(board);
            cout << endl;

            if (currentPlayer == 'X')
                makeMove(clientSocket, board);
            else
                cout << "Waiting for player O toor...";

            recv(clientSocket, &state, sizeof(state), 0);

            switch (state)
            {
            case '0':
                cout << "Unfortunately you loses !!!!" << endl;
                goto stop;
            case '1':
                cout << "Wow you wins." << endl;
                goto stop;
            case '2':
                cout << "It's a tie!" << endl;
                goto stop;
            }

            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }

    stop:
        close(clientSocket);
        //-----------------------------------
}

int runClient()
{
    char gameMode;

    cout << "Select your game mode: " << endl
         << "1. Play with server AI." << endl
         << "2. Play with other player." << endl;

    cin >> gameMode;

    char server_ip_addr[INET_ADDRSTRLEN];
    cout << "Enter the server ip address: ";
    cin >> server_ip_addr;

    int clientSocket = connectToServer(server_ip_addr, 12345);
    if (clientSocket == -1)
        return -1;

    send(clientSocket, &gameMode, sizeof(gameMode), 0);

    if (gameMode == '1')
    {
        // play with AI
        //-----------------------------------
        playerVsAI(clientSocket);
    }

    else if (gameMode == '2')
    {
        char type;

        recv(clientSocket, &type, sizeof(type), 0);

        if (type == '0')
        {
            // work as a server
            //===================================================
            close(clientSocket);

            int serverSocket;
            sockaddr_in serverAddr{}, clientAddr{};
            socklen_t addrLen = sizeof(clientAddr);

            serverSocket = (int)socket(AF_INET, SOCK_STREAM, 0);
            if (serverSocket == -1)
            {
                std::cerr << "Error creating socket\n";
                return -1;
            }

            serverAddr.sin_family = AF_INET;
            serverAddr.sin_addr.s_addr = INADDR_ANY;
            serverAddr.sin_port = htons(12346);

            if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
            {
                std::cerr << "Error binding socket\n";
                close(serverSocket);
                return -1;
            }

            if (listen(serverSocket, 2) == -1)
            {
                std::cerr << "Error listening for connections\n";
                close(serverSocket);
                return -1;
            }

            std::cout << "Waiting for player X...\n";
            clientSocket = (int)accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
            if (clientSocket == -1)
            {
                std::cerr << "Error accepting connection for player 1\n";
                close(serverSocket);
                return -1;
            }
            std::cout << "Player 'X' connected\n";

            playGame(clientSocket);
        }

        else
        {
            // work as a client
            recv(clientSocket, server_ip_addr, sizeof(server_ip_addr), 0);
            close(clientSocket);
            clientSocket = connectToServer(server_ip_addr, 12346);
            char currentPlayer = 'X';

            while (true)
            {

                char board[3][3];
                char state;

                recv(clientSocket, &board, sizeof(board), 0);
                printBoard(board);
                if (currentPlayer == 'X')
                    makeMove(clientSocket, board);
                else
                    cout << "Waiting for player O toor...";

                recv(clientSocket, &state, sizeof(state), 0);
                switch (state)
                {
                case '0':
                    cout << "Unfortunately you loses !!!!" << endl;
                    goto fin;
                case '1':
                    cout << "Wow you wins." << endl;
                    goto fin;
                case '2':
                    cout << "It's a tie!" << endl;
                    goto fin;
                }

                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }

        fin:
            close(clientSocket);
        }
    }

    else
    {
        cout << "Invalid game mode. Exiting..." << endl;
        return -1;
    }
}
