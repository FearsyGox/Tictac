#include <iostream>
using namespace std;

void printtictac(int(&board)[9]) {
	for (int i = 0; i < 9; ++i) {
		cout << board[i] << ",";

		if (((i + 1) % 3) == 0) {
			cout << endl;
		}
	}
	cout << endl;
}


void inserttictacMulti(int(&board)[9], int &player) {
	int position;

	// Give the player the infomation of what inputs the player can do
	cout << "Pick a location from 0 - 8" << endl;
	cin >> position;

	// If the Input is wrong or already existed then keep making the input until it is valid
	while (board[position] != 0) {
		printtictac(board);
		cout << "INVALID POSITION!!" << endl;
		cout << "Pick a location from 0 - 8" << endl;
		cin >> position;
	}
	// Place the input as the number the player is
	board[position] = player;
	
	// Then change the player turn to the oppent's turn
	if (player == 1) {
		player = 2;
	}
	else {
		player = 1;
	}
	
}

void inserttictacSingle(int(&board)[9], int (&player)) {
	
	// If the player is AI 
	if (player == 2) {
		int position = rand() % (9);
		// Randomize the input until there is a valid input 
		while (board[position] != 0) {
			position = rand() % (9);
		}

		// Place the input as the number the AI has choosen
		board[position] = 2;

		// Change the turn from AI to Player
		player = 1;
	}
	else {
		int position;
		// Give the player the infomation of what inputs the player can do
		cout << "Pick a location from 0 - 8" << endl;
		cin >> position;

		// If the Input is wrong or already existed then keep making the input until it is valid
		while (board[position] != 0) {
			printtictac(board);
			cout << "INVALID POSITION!!" << endl;
			cout << "Pick a location from 0 - 8" << endl;
			cin >> position;
		}
		// Place the input as the number the player is
		board[position] = 1;
		
		// Change the turn from player to AI
		player = 2;
	}
}



bool checkendgame(int(&board)[9]) {
	// check for vertical
	for (int i = 0; i < 3; i++) {
		if ((board[i + 3] == board[i]) && (board[i + 6] == board[i]) && board[i] != 0) {
			cout << "Player " << board[i] << " wins!" << endl;
			printtictac(board);
			return true;
		}
	}
	// check for horizontal
	for (int i = 0; i < 7; i = i+3) {
		if ((board[i + 1] == board[i]) && (board[i + 2] == board[i] && board[i] != 0)) {
			cout << "Player " << board[i] << " wins!" << endl;
			printtictac(board);
			return true;
		}
	}

	// check for downdiangel
	if ((board[4] == board[0]) && (board[8] == board[0]) && board[0] != 0) {
		cout << "Player " << board[0] << " wins!" << endl;
		printtictac(board);
		return true;
	}

	// check for updiangel
	if ((board[4] == board[6]) && (board[2] == board[6]) && board[6] != 0) {
		cout << "Player " << board[6] << " wins!" << endl;
		printtictac(board);
		return true;
	}

	// check for draw
	int i = 0;
	while (board[i] != 0 && i <= 10) {
		if (i == 9) {
			cout << "The match was a draw." << endl;
			printtictac(board);
			return true;
		}
		i++;

	}

	return false;
}

void pvpP2P(int(&board)[9]) {
	bool stopgame = false;
	int player = 1;

	while (!stopgame) {
		printtictac(board); // update everyone what is going on in the board
		stopgame = checkendgame(board); // check if there are winners

		// if there is no winner then give input for the person's turn
		if (stopgame == false) {
			inserttictacMulti(board, player);
		}
		

	}
}

void pvAI(int(&board)[9]) {
	bool stopgame = false;
	int player = 1;

	while (!stopgame) {
		
		printtictac(board); // update the user what is going on in the board
		stopgame = checkendgame(board); // check if there are winners
		
		//cout << player << endl;

		// if there is no winner then give input for the person's turn
		if (stopgame == false) {
			inserttictacSingle(board, player); // either it would be the player or AI Turn
		}
		
		
	}
}

int main() {
	int array[9] = { 0 };

	
	bool checking = false; // check if the game has been selected.

	
	
	while (!checking) {
		int gamemode = 0;
		cout << "Which Gamemode do you want to play?" << endl;
		cout << "1: Player Vs AI" << endl;
		cout << "2: Player Vs Player (P2P soon)" << endl;
		//cout << "3: Player Vs Player (Client and Server soon)" << endl;
		cout << "Your Input:";
		cin >> gamemode;
		cout << endl << endl;

		switch (gamemode) {
		case 1: // local
			pvAI(array);
			checking = true;
			break;
		case 2: // p2p soon 
			pvpP2P(array);
			checking = true;
			break;
		/*case 3: // Client TO server
			pvpP2PCs(array);
			checking = true;
		*/
		default:
			checking = false;
			break;
		}
		
	}
}