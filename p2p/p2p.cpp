#include "include/server.h" // startServer()
#include "include/client.h" // startClient()
#include "include/utils.h"  // detectMode()
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    bool isClient = detectMode(argc, argv);

    if (isClient)
    {
        char *ip = argv[1];
        startClient(ip);
    }
    else // server
    {
        startServer();
    }
    return 0;
}

