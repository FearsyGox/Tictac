#include <iostream>
#include <vector>
#include <cstring>
#include <thread>

#include "header.h"
#include "server.h"
#include "client.h"
#include "game.h"

using namespace std;

int main() {

    int mode;
    char type;
    char gameMode;

    cout << "Select mode: " << endl
         << "1. Run as a server." << endl
         << "2. Run as client." << endl;
    cin >> mode;

    // Server mode
    if (mode == 1) {

        vector<thread> threads;

        int serverSocket, clientSocket;
        sockaddr_in serverAddr{}, clientAddr{};
        socklen_t addrLen = sizeof(clientAddr);

        serverSocket = (int) socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            std::cerr << "Error creating socket\n";
            return -1;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(12345);

        if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
            std::cerr << "Error binding socket\n";
            close(serverSocket);
            return -1;
        }

        if (listen(serverSocket, 2) == -1) {
            std::cerr << "Error listening for connections\n";
            close(serverSocket);
            return -1;
        }
        
        
        vector<int> clients;
        while (true)
        {
            std::cout << "Waiting for player ...\n";
            clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
            if (clientSocket == -1) {
                std::cerr << "Error accepting connection for player \n";
                close(serverSocket);
                return -1;
            }
            std::cout << "New player connected\n";

            // receive player gameMode
            //---------------------------------
            recv(clientSocket, &gameMode, sizeof(gameMode), 0);

            if (gameMode == '1')
                threads.emplace_back(playGame_, clientSocket);

            else{ // play  with player
                if (clients.empty())
                {
                    clients.push_back(clientSocket);
                }
                else{

                    // send client2's ip to client2
                    //----------------------------------
                    char ser = '0', cl = '1';
                    send(clients[0], &ser, sizeof(ser), 0);
                    send(clientSocket, &cl, sizeof(cl), 0);
                    // send ip address 
                    char ipAddress[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(clientAddr.sin_addr), ipAddress, INET_ADDRSTRLEN);
                    send(clientSocket, ipAddress, sizeof(ipAddress), 0);

                    close(clients[0]);
                    close(clientSocket);
                    clients.pop_back();
                }
                
                //-----------------------------------
            }
            
        }
        close(serverSocket);
    }

    // Client mode
    else if (mode == 2) {

        cout << "Select your game mode: " << endl
             << "1. Play with server AI." << endl
             << "2. Play with other player." << endl;

        cin >> gameMode;

        char server_ip_addr[INET_ADDRSTRLEN];
        cout << "Enter the server ip address: ";
        cin >> server_ip_addr;

        int clientSocket = connectToServer(server_ip_addr, 12345);
        if (clientSocket == -1) return -1;


        send(clientSocket, &gameMode, sizeof(gameMode),0);

        if (gameMode == '1'){
            // play with AI
            //-----------------------------------
            char currentPlayer = 'X';

            while (true) {

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

                switch (state) {
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

        else if (gameMode == '2'){

            recv(clientSocket, &type, sizeof(type), 0);

            if (type == '0') {
                // work as a server
                //===================================================
                close(clientSocket);

                int serverSocket;
                sockaddr_in serverAddr{}, clientAddr{};
                socklen_t addrLen = sizeof(clientAddr);

                serverSocket = (int) socket(AF_INET, SOCK_STREAM, 0);
                if (serverSocket == -1) {
                    std::cerr << "Error creating socket\n";
                    return -1;
                }

                serverAddr.sin_family = AF_INET;
                serverAddr.sin_addr.s_addr = INADDR_ANY;
                serverAddr.sin_port = htons(12346);

                if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
                    std::cerr << "Error binding socket\n";
                    close(serverSocket);
                    return -1;
                }

                if (listen(serverSocket, 2) == -1) {
                    std::cerr << "Error listening for connections\n";
                    close(serverSocket);
                    return -1;
                }

                std::cout << "Waiting for player X...\n";
                clientSocket = (int) accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
                if (clientSocket == -1) {
                    std::cerr << "Error accepting connection for player 1\n";
                    close(serverSocket);
                    return -1;
                }
                std::cout << "Player 'X' connected\n";

                playGame(clientSocket);
            }

            else{
                // work as a client
                recv(clientSocket, server_ip_addr, sizeof(server_ip_addr), 0);
                close(clientSocket);
                clientSocket = connectToServer(server_ip_addr, 12346);
                char currentPlayer = 'X';

                while (true) {

                    char board[3][3];
                    char state;

                    recv(clientSocket, &board, sizeof(board), 0);
                    printBoard(board);
                    if (currentPlayer == 'X')
                        makeMove(clientSocket, board);
                    else
                        cout << "Waiting for player O toor..."; 

                    recv(clientSocket, &state, sizeof(state), 0);
                    switch (state) {
                        case '0':
                            cout << "Unfortunately you loses !!!!" << endl;
                            goto fin;
                        case '1':
                            cout << "Wow you wins." << endl;
                            goto fin;
                        case '2':
                            cout << "It's a tie!" << endl; goto fin;
                    }

                    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                }

                fin:
                close(clientSocket);

            }
        }

        else{
            cout << "Invalid game mode. Exiting..." << endl;
            return -1;
        }

    }
    
    else {
        std::cerr << "Invalid mode. Exiting...\n";
        return -1;
    }

    return 0;
}
