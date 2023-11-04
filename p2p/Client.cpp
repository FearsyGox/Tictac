#include "Client.h"

#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

char *request(struct Client *client, char *server_ip, void *request, unsigned long size);

struct Client client_constructor(int domain, int service, int protocol, int port, u_long interface)
{
    // Create client
    struct Client client;
    client.domain = domain;
    client.port = port;
    client.interface = interface;

    // Establish a socket connection.
    client.socket = socket(domain, service, protocol);
    client.request = request;

    // Return the constructed socket.
    return client;
}

char *request(struct Client *client, char *server_ip, void *request, unsigned long size)
{
    // Create an address struct for the server.
    struct sockaddr_in server_address;

    // Copy the client's parameters to this address.
    server_address.sin_family = client->domain;
    server_address.sin_port = htons(client->port);
    server_address.sin_addr.s_addr = (int)client->interface;

    // Make the connection.
    inet_pton(client->domain, server_ip, &server_address.sin_addr); // Convert the ip address to binary.

    if (connect(client->socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Failed to connect to server...\n");
        exit(1);
    }

    // Send the request
    if (send(client->socket, request, size, 0) < 0)
    {
        perror("Failed to send request...\n");
        exit(1);
    }

    // Read the response.
    char *response = new char[30000];
    if (read(client->socket, request, size) < 0)
    {
        perror("Failed to read response...\n");
        exit(1);
    }
    return response;
}