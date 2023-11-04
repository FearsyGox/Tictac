// client struct is used a s a tool for connecting to and interacting with servers
// client objects are used to send requests to server

#ifndef Client_h
#define Client_h

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

struct Client
{
    // network socket for handling connections
    int socket;

    int domain;
    int service;
    int protocol;
    int port;
    unsigned long interface;

    // request method allows a client to make a request of a specified server
    char *(*request)(struct Client *client, char *serverr_ip, void *request, unsigned long size);
};

struct Client client_constructor(int domain, int service, int protocol, int port, unsigned long interface);

#endif