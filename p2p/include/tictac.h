#ifndef TICTAC_H
#define TICTAC_H

#include <iostream>
using namespace std;

void printtictac(int (&board)[9])
{
	for (int i = 0; i < 9; ++i)
	{
		if (board[i] == 0)
		{
			cout << "-";
		}else if(board[i] == 1)
		{
			cout << "X";
		}else if(board[i] == 2)
		{
			cout << "O";
		}
	
		if (((i + 1) % 3) == 0)
		{
			cout << endl;
			continue;
		}

		cout << " | ";
	}
}

void displayRules(char letter)
{
	cout << endl
		 << "Game started: " << endl;
	cout << "Rules: " << endl;
	cout << endl
		 << "on your turn ONLY... type 'done' to quit.." << endl;
	cout << "You are " << letter << "'s, to make a move type the corresponding number" << endl;
	for(int i = 1; i < 10; i++)
	{
		cout << i;
		if(i % 3 == 0)
		{
			cout << endl;
			continue;
		}
		cout << " | ";
	
	}
	cout << endl;
}

void inserttictac(int (&board)[9])
{
	int position;

	cout << "Pick a location from 0 - 8" << endl;
	cin >> position;
	while (board[position] != 0)
	{
		printtictac(board);
		cout << "INVALID POSITION!!" << endl;
		cout << "Pick a location from 0 - 8" << endl;
		cin >> position;
	}
	board[position] = 1;
}

void randomAI(int (&board)[9])
{
	int position = rand() % (9);
	while (board[position] != 0)
	{
		position = rand() % (9);
	}
	board[position] = 2;
}

bool checkendgame(int (&board)[9])
{
	// check for vertical
	for (int i = 0; i < 3; i++)
	{
		if ((board[i + 3] == board[i]) && (board[i + 6] == board[i]) && board[i] != 0)
		{
			cout << "Player " << board[i] << " wins!" << endl;
			printtictac(board);
			return true;
		}
	}
	// check for horizontal
	for (int i = 0; i < 7; i = i + 3)
	{
		if ((board[i + 1] == board[i]) && (board[i + 2] == board[i] && board[i] != 0))
		{
			cout << "Player " << board[i] << " wins!" << endl;
			printtictac(board);
			return true;
		}
	}

	// check for downdiangel
	if ((board[4] == board[0]) && (board[8] == board[0]) && board[0] != 0)
	{
		cout << "Player " << board[0] << " wins!" << endl;
		printtictac(board);
		return true;
	}

	// check for updiangel
	if ((board[4] == board[6]) && (board[2] == board[6]) && board[6] != 0)
	{
		cout << "Player " << board[6] << " wins!" << endl;
		printtictac(board);
		return true;
	}

	// check for draw
	int i = 0;
	while (board[i] != 0 && i <= 10)
	{
		if (i == 9)
		{
			cout << "The match was a draw." << endl;
			printtictac(board);
			return true;
		}
		i++;
	}

	return false;
}

void gamemode(int (&board)[9])
{
	bool stopgame = false;

	while (!stopgame)
	{
		printtictac(board);

		stopgame = checkendgame(board); // Check if the AI wins

		if (stopgame == false)
		{
			inserttictac(board);			// player input
			stopgame = checkendgame(board); // check if the Player wins
			randomAI(board);				// AI input
		}
	}
}

// int main() {
// 	int array[9] = { 0 };

// 	gamemode(array);
// }

#endif