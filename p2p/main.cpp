//#include "../libeom.h"
#include "Server.cpp"
#include "Client.cpp"
#include <pthread.h>
#include <thread>
#include <bits/stdc++.h>    // memset
#include <unistd.h>

using namespace std;


void server_function()
{
    printf("server running...\n");

    // Port 1248, Backlog 20
    // AF_INET - ipv4, AF_INET6 - ipv6
    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 1248, 20);

    // cast server.address to a sockaddr, then create a sockaddr pointer to it
    struct sockaddr *address = (struct sockaddr *)&server.address;    
    
    // cast the size of the address to a socklen_t
    socklen_t address_length = (socklen_t)sizeof(server.address);


    // While loop to accept incoming connections
    // 1. Accept incoming connect, s
    // 2. Save what is sent in the request
    // 3. Close the connection
    // 4. Repeat

    while (1)   // infinite loop
    {
        // accepting the clients information to establish connection
        int client = accept(server.socket, address, &address_length);

        // this may need to be changed to a single character
        // if this is the case the program could be simplified
        char request[255];
        memset(request, 0, 255);    // clear the memory to avoid errors
        read(client, request, 255);
        cout << request << endl;
        close(client);
    }
}

void client_function(char *request)
{
    struct Client client = client_constructor(AF_INET, SOCK_STREAM, 0, 1248, INADDR_ANY);   // INADDR_ANY allows binding on any socket
    
    // 127.0.0.1 is loopback address to self, localhost
    char server_ip[] = "127.0.0.1";
    client.request(&client, server_ip, request, 255);
}

// void client_function()
// {
//     // Port 1248, Backlog 20
//     // AF_INET - ipv4, AF_INET6 - ipv6
//     struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 1248, 20);

//     // cast server.address to a sockaddr, then create a sockaddr pointer to it
//     struct sockaddr *address = (struct sockaddr *)&server.address;    
    
//     // cast the size of the address to a socklen_t
//     socklen_t address_length = (socklen_t)sizeof(server.address);
// }

int main()
{
    cout << "starting server...\n";
    thread server_thread;
    server_thread = thread(server_function);

    while(1)
    {
        char request[255];
        memset(request, 0, 255);
        fgets(request, 255, stdin);
        client_function(request);

    }

    return 0;
    
}