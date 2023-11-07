// utils.h

#ifndef UTILS_H
#define UTILS_H

#include <regex>

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
        std::regex ip_pattern("^(?:(25[0-5]|2[0-4][0-9]|1[0-9]{2}|[1-9]?[0-9])(/.(?!$)|$)){4}$");
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

#endif