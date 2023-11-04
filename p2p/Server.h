/*
The Server struct is used as the basis for noeds that need to operate as servers.
Server connects to the network and listens on a given port.
*/

// #include "../libeom.h"

#ifndef Server_h
#define Server_h

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>


// Define Server struct
struct Server
{
    int domain;
    int service;
    int protocol;
    std::string interface;
    int port;
    int backlog;
    struct sockaddr_in address;
    int socket;
};

// Constructors
struct Server server_constructor(int domain, int service, int protocol, std::string interface, int port, int backlog);

#endif