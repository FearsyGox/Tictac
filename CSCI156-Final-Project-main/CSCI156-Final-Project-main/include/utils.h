// utils.h

#ifndef UTILS_H
#define UTILS_H

#include <regex>
#include <limits>
#include <iostream>


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

bool getMove(int *board, char *message, int maxSize)
{
    cout << "Enter move: ";

    bool valid = false;
    char temp;

    while (!valid)
    {
        valid = true;
        cin.getline(message, maxSize);

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again." << endl;
            valid = false;
            continue;
        }

        if (strlen(message) == 4 && strncmp(message, "done", 4) == 0)
        {
            return true;
        }

        if (strlen(message) != 1)
        {
            cout << "Invalid move. Enter a single digit (1-9):" << endl;
            valid = false;
            continue;
        }

        temp = message[0] - '0';
        if (temp < 1 || temp > 9)
        {
            cout << "Invalid move. Enter a digit from 1 to 9:" << endl;
            valid = false;
            continue;
        }

        int index = temp - 1;
        if (board[index] != 0)
        {
            cout << "Square is occupied. Please choose an empty square:" << endl;
            valid = false;
        }
    }

    return true;
}

#endif