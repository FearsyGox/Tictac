#include <iostream>
#include <vector>
#include <thread>
#include <unordered_set>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "game.h"

#include "header.h"

using namespace std;

// GLOBAL VARS
const int SERVER_SOCKET = 12345;

struct Client
{
    sockaddr_in* addr;
    socklen_t addr_len;
    int socket;
};

queue<Client*> waitingList;
mutex waitingListMutex;

void addClientToWaitingList(Client* client)
{
    unique_lock<mutex> mlock(waitingListMutex);
    waitingList.push(client);
}

Client* getClientFromWaitingList()
{
    unique_lock<mutex> mlock(waitingListMutex);
    Client* client = waitingList.front();
    waitingList.pop();
    return client;
}

void setupGame(Client *clientA){
        
        
        //! NEED LOGIC HERE TO TO ESTABLISH CONNECTION BETWEEN CLIENTS

        // send necessary information to clientA and clientB to start game.

        Client* clientB = getClientFromWaitingList();
    
        //! OLD CODE BELOW
        
        
        // send client2's ip to client2
        //----------------------------------
        // char ser = '0', cl = '1';

        // int ClientSocket = waitingList.front();
        // send(clients[0], &ser, sizeof(ser), 0);
        // send(clientSocket, &cl, sizeof(cl), 0);
        // // send ip address
        // char ipAddress[INET_ADDRSTRLEN];
        // inet_ntop(AF_INET, &(clientAddr.sin_addr), ipAddress, INET_ADDRSTRLEN);
        // send(clientSocket, ipAddress, sizeof(ipAddress), 0);

        // close(clients[0]);
        // close(clientSocket);
        // clients.pop_back();
}

int handleThread(Client* client)
{
    char gameMode;

    std::cout << "New player connected\n";

    // receive player gameMode
    //---------------------------------
    recv(client->socket, &gameMode, sizeof(gameMode), 0);

    if (gameMode == '1')
    {   // play game with AI
        playGame_(client->socket);
    }
    else if (waitingList.empty())
    {   // add player to queue
        addClientToWaitingList(client);
    }
    else
    {   // play game with another player
        setupGame(client);
    }
    return 0;
}

int runServer()
{
    vector<thread> threads;

    int serverSocket, clientSocket;
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
    serverAddr.sin_port = htons(SERVER_SOCKET);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Error binding socket\n";
        close(serverSocket);
        return -1;
    }

    if (listen(serverSocket, 10) == -1)
    {
        std::cerr << "Error listening for connections\n";
        close(serverSocket);
        return -1;
    }

    // accept new clients
    while (true)
    {
        std::cout << "Waiting for player ...\n";
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
        if (clientSocket == -1)
        {
            std::cerr << "Error accepting connection for player \n";
            close(serverSocket);
            return -1;
        }

        // create new Client
        Client* client = new Client();
        client->addr = &clientAddr;
        client->addr_len = addrLen;
        client->socket = clientSocket;

        // create new thread
        thread th = thread(handleThread, client);
        th.detach();
    }

    close(serverSocket);
    return 0;
}
