#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

// Constructors

struct Server server_constructor(int domain, int service, int protocol, unsigned long interface, int port, int backlog)
{
    struct Server server;
    
    // Define basic parameters of the server.
    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;

    // Construct the server using paramaters
    server.address.sin_family = domain;
    server.address.sin_port = htons(port);

    // Create Socket
    // The socket allows the server to connect to the network
    server.socket = socket(domain, service, protocol);

    // Confimrm conection was successful
    if (server.socket == 0)
    {
        perror("Failed to connect socket....\n");
        exit(1);
    }

    // Attempt to bind the socket to the network
    if ((bind(server.socket, (struct sockaddr *)&server.address, sizeof(server.address)))<0)
    {
        perror("Failed to bind socket...\n");
        exit(1);
    }

    // Start listenening on the network with the socket and a given backlog
    if ((listen(server.socket, server.backlog))<0)
    {
        perror("Failed to start listening...\n");
        exit(1);
    }
    return server;
}