// Both programs can run this code and communicate with each other
// We need to know the ip of the other memeber and add it in the client function.

// #include "../libeom.h"
#include "Server.cpp"
#include "Client.cpp"
#include <pthread.h>
#include <thread>
#include <bits/stdc++.h> // memset
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;
bool detectMode(int argc, char *argv[]); // returns true if client, false if server

void client_function(char *request);
void server_function();

int main(int argc, char *argv[])
{
    bool isClient = false;

    isClient = detectMode(argc, argv);

    if (isClient)
    {
        return 0;
    }
    else // server
    {
        server_function();
    }

    while (1)
    {
        char request[255];
        memset(request, 0, 255);
        fgets(request, 255, stdin);
        client_function(request);
    }

    return 0;
}

void server_function()
{
    printf("server running...\n");

    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 1248, 20);
    struct sockaddr *address = (struct sockaddr *)&server.address;
    socklen_t address_length = (socklen_t)sizeof(server.address);

    while (1) // infinite loop
    {
        int client = accept(server.socket, address, &address_length);
        char request[255];
        memset(request, 0, 255); 
        read(client, request, 255);

        printf("\t\tClient says: %s\n", request);
        close(client);
    }
}

void client_function(char *request)
{
    struct Client client = client_constructor(AF_INET, SOCK_STREAM, 0, 1248, INADDR_ANY); // INADDR_ANY allows binding on any socket

    // 127.0.0.1 is loopback address to self, localhost
    char server_ip[] = "127.0.0.1";
    client.request(&client, server_ip, request, 255);
}

bool detectMode(int argc, char *argv[])
{
    if (argc > 2)
    {
        printf("Too many arguments...\n");
        exit(1);
    }
    else if (argc == 2)
    {
        printf("Client mode...\n");
        return true;
        // check if ip is valid
        regex ip_pattern("^(?:(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])(/.(?!$)|$)){4}$");
        if (regex_match(argv[1], ip_pattern))
        {
            printf("Client mode...\n");
            return true;
        }
        else
        {
            printf("Invalid ip address...\n");
            exit(1);
        }
    }
    else
    {
        printf("Server mode...\n");
        return false;
    }
}