/*
Final Project: Black Jack
Programer: Sachin Kumar
Date: 19/08/2022

Description: This program will simulate a real game of Black Jack with a dealer vs human, 
another mode for human vs human. This Blackjack game resembles the casino version of this 
game where betting takes place and there is a dealer with other players or just one. 
Place your bets and let�s begin� 21� BlackJack!

Mods:

*/

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <algorithm>
#include <ctime>
#include <vector>

struct Player {
	std::string userName = "";
	unsigned int score = 0;
	unsigned int hand = 0;
	unsigned int bet = 0;
};

// Function Prototypes
void userAccount(int x, Player users[]);
void menu(bool CardsDealt[], int HouseHand[], int PlayerHand[], int HouseCardCount, int PlayerCardCount, int x, Player users[]);
void gameComputer(bool CardsDealt[], int HouseHand[], int PlayerHand[], int HouseCardCount, int PlayerCardCount, int x, Player users[]);
void gameUser(bool CardsDealt[], int HouseHand[], int PlayerHand[], int HouseCardCount, int PlayerCardCount, int x, Player users[]);
void scores(int x, Player users[]);
void shuffle(bool CardDelt[]);
void displayCard(int Card);
void displayHand(int Hand[], const int CardCount);
void displayScoresAndHands(int HouseHand[], const int HouseCardCount, int PlayerHand[], const int PlayerCardCount);
int nextCard(bool CardsDealt[]);
int scoreHand(int Hand[], const int CardCount);

using namespace std;

//=============================
//		   main()		      =
// The section for BlackJack  =
//=============================
int main()
{
	// For random numbers
	time_t qTime;
	time(&qTime);
	srand(qTime);

	// Declarations
	int count = 0;
	bool CardsDealt[52];
	int HouseCardCount = 0;
	int HouseHand[12];
	int PlayerCardCount = 0;
	int PlayerHand[12];
	int x = 0;
	const int PLAYERS = 3;
	Player users[PLAYERS];


	// Game starts with the user account
	userAccount( x, users);

	// The menu for which game mode to play in
	menu(CardsDealt, HouseHand, PlayerHand, HouseCardCount, PlayerCardCount, x, users);
	
	// For windows to show final screen
	system("pause");
	return 0; // Return success
}

//=============================
//		   userAccount()	  =
// Where the user will enter  =
// his or her account or      =
// create a new one           =
//=============================
void userAccount(int x, Player users[])
{
	fstream inData;
	fstream inScores;
	string userAndPass; // For the read data from the file
	string passWord, line; // For the entered data by the user
	string userName;
	char option;
	bool found = false;


	// The display
	cout << "===========================================" << endl;
	cout << "=         Black Jack User Accounts        =" << endl;
	cout << "===========================================\n\n" << endl;

	// The Menu for creating the user account or using an existing one
	cout << "Please select one of the options for player #" << x << "\n" <<
		"1. Create a new account\n2. Use an existing account" << endl;
	cout << "Option: ";
	cin >> option;
	system("cls");

	switch (option)
	{
	case '1':
		cout << "===========================================" << endl;
		cout << "=            Black Jack New User          =" << endl;
		cout << "===========================================\n\n" << endl;

		inData.open("Username.dat", ios::app); // Stores username and password
		inScores.open("Scores.dat", ios::app); // Stores the username and score
		cout << "Enter Username (lower case only): ";
		std::cin >> userName;
		// Changes the username to lowercase if entered incorrectly
		std::transform(userName.begin(), userName.end(), userName.begin(), ::tolower);
		cout << endl;
		users[x].userName = userName; // Inputs the username into the memory
		users[x].score = 1000; // Creates money for the new user

		cout << "Enter Password: ";
		cin >> passWord;
		cout << endl;

		inData << userName << " " << passWord << endl;
		inScores << userName << endl << users[x].score << endl;

		inData.close();
		inScores.close();

		cout << "Registering the user to the system!";
		cout << "\n\n *";
		for (int a = 1; a<8; a++) // Change 'a<?' to how many * you want
		{
			cout << "*";
			Sleep(500);
		}
		cin.sync(); cin.ignore();
		system("cls");

		cout << "Registered Successfully." << endl;
		Sleep(1000);
		system("cls");
		break;

	case '2':

		cout << "===========================================" << endl;
		cout << "=         Black Jack Existing User        =" << endl;
		cout << "===========================================\n\n" << endl;
	
		inData.open("Username.dat", ios::in);
		// Entering the current username
		cout << "Please enter your username (lower case only): ";
		std::cin >> userName;
		cout << "\nPlease enter your password (lower and upper case): ";
		cin >> passWord;
		users[x].userName = userName;

		
		// Changes the username to lowercase if entered incorrectly
		std::transform(userName.begin(), userName.end(), userName.begin(), ::tolower);

		userAndPass = userName + " " + passWord;

		if (inData.is_open()) {
			while (getline(inData, line) && !found) {
				if (line.compare(userAndPass) == 0) { //match strings exactly!
					found = true; // found is true => break loop
				}
			}
			inData.close(); //close the file before recursivly opening it again later
			if (found) {
				cout << "\nWelcome " << userName << "!\n";
				Sleep(2000);
				system("cls");
			}
			else {
				cout << "You entered a username or password not in the system. Press '1' to exit." << endl;
				cin >> passWord;
				if (passWord == "1")
					system("cls");
				userAccount(x, users);
			}
			found = false;

			inScores.open("Scores.dat", ios::in); // Stores the username and score
			if (inScores.is_open()) {
				while (getline(inScores, line) && !found) {
					if (line.compare(userName) == 0) { //match strings exactly!
						found = true; // found is true => break loop
						inScores >> users[x].score;
					}
				}
				inData.close(); //close the file before recursivly opening it again later
				inScores.close();
			}
		}
		
		break;

	default:
		cout << "You entered the wrong selection. Try again.\n";
		userAccount(x, users);
		break;
	}
}

//=============================
//		     menu()		      =
// Selection to play with     =
// computer or another user   =
//=============================
void menu(bool CardsDealt[], int HouseHand[], int PlayerHand[], int HouseCardCount, int PlayerCardCount, int x, Player users[])
{
	// Asks the user how it would like to play
	char option;

	cout << "===========================================" << endl;
	cout << "=         Black Jack Mode Selection       =" << endl;
	cout << "===========================================\n\n" << endl;

	cout << "Would you like to...\n";
	// Playing with another user is still in the works, computer is most stable.
	cout << "1. Play the computer\n2. Play with another user." << endl;
	cin >> option;

	switch (option)
	{
	case '1':
		// Function Prototype start game
		system("cls");
		cout << "Preparing game with the computer!";
		cout << "\n\n *";
		for (int a = 1; a<8; a++) // Change 'a<?' to how many * you want
		{
			cout << "*";
			Sleep(500);
		}
		cin.sync(); cin.ignore();
		system("cls");
		gameComputer(CardsDealt, HouseHand, PlayerHand, HouseCardCount, PlayerCardCount, x, users);
		break;

	case '2':
		// Function Prototype start game
		system("cls");
		cout << "Preparing game with other users!";
		cout << "\n\n *";
		for (int a = 1; a<8; a++) // Change 'a<?' to how many * you want
		{
			cout << "*";
			Sleep(500);
		}
		cin.sync(); cin.ignore();
		system("cls");
		gameUser(CardsDealt, HouseHand, PlayerHand, HouseCardCount, PlayerCardCount, x, users);
		break;

	default:
		break;
	}
}

//=============================
//		   gameComputer()	  =
//The Game With the computer  =
//=============================
void gameComputer(bool CardsDealt[], int HouseHand[], int PlayerHand[], int HouseCardCount, int PlayerCardCount, int x, Player users[])
{

	bool flag = true;
	int choice = 1;
	int bets;

	// Loops for hand
	while (flag = true) {

		// Shuffles the cards
		shuffle(CardsDealt);

		// Two cards are delt for each player and house
		PlayerHand[0] = nextCard(CardsDealt);
		HouseHand[0] = nextCard(CardsDealt);
		PlayerHand[1] = nextCard(CardsDealt);
		HouseHand[1] = nextCard(CardsDealt);
		HouseCardCount = 2;
		PlayerCardCount = 2;

		char PlayerChoice;
		bool PlayerHits = true;
		int PlayerScore = scoreHand(PlayerHand, PlayerCardCount);

		// The start of a new hand
		cout << "===========================================" << endl;
		cout << "=             Black Jack 21               =" << endl;
		cout << "===========================================" << endl;

		cout << users[x].userName << "'s Money: $" << users[x].score << endl;

		cout << "Place your bets!" << endl;
		cout << "Bet: ";
		cin >> bets;
		system("cls");

		while (users[x].bet > users[x].score) {  // Validation if user enters more money than in account
			cout << "You entered a bet more than you have in your account.\n"
				<< "Please enter your bet: ";
			cin >> users[x].bet;
		}

		system("cls");

		// The start of a new hand
		cout << "===========================================" << endl;
		cout << "=             Black Jack 21               =" << endl;
		cout << "===========================================" << endl;

		//=============================== Fix validation on money bet====================================
		users[x].score = users[x].score - bets;
		cout << "You bet: $" << bets << "	Money left: $" << users[x].score << endl << endl;

		// Get Player's hits. Calculate the score and redisplay after each hit.
		do {

			// Displays the dealt cards, but only the House's second card.
			cout << "House's Hand" << endl;
			cout << "** ";
			displayCard(HouseHand[1]);
			cout << endl;
			cout << "\nPlayer's Hand: Score = " << scoreHand(PlayerHand, PlayerCardCount) << endl;
			displayHand(PlayerHand, PlayerCardCount);

			// Ask the Player whether he wants a hit or to stay
			if (users[x].score > users[x].bet) {
				cout << "\nHit(h) stay(s) or double(d): ";
				cin >> PlayerChoice;
			}
			else {
				cout << "\nHit(h) or stay(s): ";
				cin >> PlayerChoice;
			}
			if (PlayerChoice == 'h') {
				PlayerHand[PlayerCardCount] = nextCard(CardsDealt);
				++PlayerCardCount;
			}
			else if (PlayerChoice == 's') {
				PlayerHits = false;
			}
			else if ((PlayerChoice == 'd') && (users[x].score > users[x].bet)) {
				PlayerHand[PlayerCardCount] = nextCard(CardsDealt);
				++PlayerCardCount;
				users[x].bet = users[x].bet * 2;
				PlayerHits = false;
			}
			else {
				cout << "Error: Try Again!" << endl;
			}
			cout << endl;

			// Get the Player's current score to update and check for bust.
			PlayerScore = scoreHand(PlayerHand, PlayerCardCount);
		} while (PlayerHits && PlayerScore < 22);

		// Once the player is done, a check is taken place to see if  busted
		if (PlayerScore > 21) {

			// The Player busted. The House wins.
			system("cls");
			scores(x, users);
			cout << "The House Wins!" << endl;
			cout << "You lost $" << bets << endl;
			displayScoresAndHands(HouseHand, HouseCardCount, PlayerHand, PlayerCardCount);
		}
		else {

			// If the player didn't bust, then the house takes hits below 17
			int HouseScore = scoreHand(HouseHand, HouseCardCount);
			while (HouseScore < 17) {
				HouseHand[HouseCardCount] = nextCard(CardsDealt);
				++HouseCardCount;
				HouseScore = scoreHand(HouseHand, HouseCardCount);
			}

			bool HouseBusts = (HouseScore > 21);
			if (HouseBusts) {
				// The House busted. Player wins
				system("cls");
				scores(x, users);
				cout << "You Win!" << endl;
				bets = bets * 2;
				cout << "You gained $" << bets << endl;
				users[x].score = bets + users[x].score;
				displayScoresAndHands(HouseHand, HouseCardCount, PlayerHand, PlayerCardCount);
			}
			else {

				// Compare scores and determine the winner
				if (PlayerScore == HouseScore) {

					// Tie. This is called a "push."
					system("cls");
					scores(x, users);
					cout << "Tie!" << endl;
					cout << "You gained $0" << endl;
					users[x].score = bets + users[x].score;
					displayScoresAndHands(HouseHand, HouseCardCount, PlayerHand, PlayerCardCount);
				}
				else if (PlayerScore > HouseScore) {

					// The Player wins
					system("cls");
					scores(x, users);
					cout << "You Win!" << endl;
					bets = bets * 2;
					cout << "You gained $" << bets << endl;
					users[x].score = bets + users[x].score;
					displayScoresAndHands(HouseHand, HouseCardCount, PlayerHand, PlayerCardCount);
				}
				else {

					// The House wins
					system("cls");
					scores(x, users);
					cout << "The House Wins!" << endl;
					cout << "You lost $" << bets << endl << endl;
					displayScoresAndHands(HouseHand, HouseCardCount, PlayerHand, PlayerCardCount);
				}
			}
		}

		system("pause");
		system("cls");
		cout << "Would you like to play another game or end? yes(1) or no(2) " << endl;
		cin >> choice;
		while (!((choice == 1) || (choice == 2))) { // Validation if ented wrong item
			cout << "You entered the wrong choice.\nPlease enter again: ";
			cin >> choice;
		}

		if (choice == 2) {
			flag = false;
		}
		else {
			flag = true;
			system("cls");
		}
	}
}

//=============================
//		   gameUser()		  =
//The Game With Another User  =
//=============================
void gameUser(bool CardsDealt[], int HouseHand[], int PlayerHand[], int HouseCardCount, int PlayerCardCount, int x, Player users[])
{

	// StrLine for entering players  
	std::string StrLine = "";
	// Used as an index for VecPlayers
	unsigned int CountPlayer = 0;
	int i;
	int bets = 0;

	cout << "How many players are going to play?\n(1 to 2) Players Only, not including main user: ";
	cin >> i;
	while ((i < 1) || (i > 2)) {
		cout << "You entered a incorrect value. Please enter again: ";
		cin >> i;
	}

	for (int x = 1; x <= i; x++) {
		cout << "Enter user information for account " << x << endl;
		userAccount(x, users);
	}

	bool flag = true;
	int choice = 1;

	while (flag == true) {

		// Shuffles the cards
		shuffle(CardsDealt);

		// Two cards are delt for each player and house
		HouseHand[0] = nextCard(CardsDealt);
		HouseHand[1] = nextCard(CardsDealt);
		HouseCardCount = 2;

		for (int x = 0; x <= i; x++) {
			// The start of a new hand
			cout << "===========================================" << endl;
			cout << "=             Black Jack 21               =" << endl;
			cout << "===========================================" << endl;

			cout << users[x].userName << "'s Money: $" << users[x].score << endl;

			cout << "Place your bets!" << endl;
			cout << "Bet: ";
			cin >> users[x].bet;
			while (users[x].bet > users[x].score) {  // Validation if user enters more money than in account
				cout << "You entered a bet more than you have in your account.\n"
					<< "Please enter your bet: ";
				cin >> users[x].bet;
			}

			system("cls");
		}
		// Gathers the total bets
		for (int x = 0; x <= i; x++) {
			bets += users[x].bet;
		}

		for (int x = 0; x <= i; x++) {
			PlayerHand[0] = nextCard(CardsDealt);
			PlayerHand[1] = nextCard(CardsDealt);
			PlayerCardCount = 2;

			char PlayerChoice;
			bool PlayerHits = true;
			int PlayerScore = scoreHand(PlayerHand, PlayerCardCount);

			// The start of a new hand
			cout << "===========================================" << endl;
			cout << "=             Black Jack 21               =" << endl;
			cout << "===========================================" << endl;

			users[x].score = users[x].score - users[x].bet;
			cout << users[x].userName << " bet: $" << users[x].bet << "	Money left: $" << users[x].score << endl << endl;

			// Get Player's hits. Calculate the score and redisplay after each hit.
			do {

				// Displays the dealt cards, but only the House's second card.
				cout << "House's Hand" << endl;
				cout << "** ";
				displayCard(HouseHand[1]);
				cout << endl;
				cout << "\nPlayer's Hand: Score = " << scoreHand(PlayerHand, PlayerCardCount) << endl;
				displayHand(PlayerHand, PlayerCardCount);

				// Ask the Player whether he wants a hit or to stay
				if (users[x].score > users[x].bet) {
					cout << "\nHit(h) stay(s) or double(d): ";
					cin >> PlayerChoice;
				}
				else {
					cout << "\nHit(h) or stay(s): ";
					cin >> PlayerChoice;
				}
				if (PlayerChoice == 'h') {
					PlayerHand[PlayerCardCount] = nextCard(CardsDealt);
					++PlayerCardCount;
				}
				else if (PlayerChoice == 's') {
					PlayerHits = false;
				}
				else if ((PlayerChoice == 'd') && (users[x].score > users[x].bet)) {
					PlayerHand[PlayerCardCount] = nextCard(CardsDealt);
					++PlayerCardCount;
					users[x].bet = users[x].bet * 2;
					PlayerHits = false;
				}
				else {
					cout << "Error: Try Again!" << endl;
				}
				cout << endl;

				// Get the Player's current score to update and check for bust.
				PlayerScore = scoreHand(PlayerHand, PlayerCardCount);

			} while (PlayerHits && PlayerScore < 22);
			users[x].hand = PlayerScore;
			if (users[x].hand > 21) {
				cout << users[x].userName << " Busted!" << endl;
				users[x].hand = 0;
				Sleep(1500);
			}
			if (x != i) {
				system("cls");
				cout << "Next Players Turn\nOnce ready ";
				system("pause");
			}
			else {
				cout << "End of the game. To see scores ";
				system("pause");
				system("cls");
			}

		}

		// Once the player is done, a check is taken place to see if busted
			// The house takes hits below 17
		int HouseScore = scoreHand(HouseHand, HouseCardCount);
		while (HouseScore < 17) {
			HouseHand[HouseCardCount] = nextCard(CardsDealt);
			++HouseCardCount;
			HouseScore = scoreHand(HouseHand, HouseCardCount);
		}

		bool HouseBusts = (HouseScore > 21);
		if (HouseBusts) {
			// The House busted. Checks for a tie
			system("cls");
			cout << "===========================================" << endl;
			cout << "=         Black Jack Round Finished       =" << endl;
			cout << "===========================================\n\n" << endl;
			if (i == 1) {
				if (users[0].hand == users[1].hand) {
					cout << "Tie!";
					cout << "You both gained $0" << endl;
					users[0].score = users[0].bet + users[0].score + users[0].score;
					users[1].score = users[1].bet + users[1].score + users[1].score;
				}
				if (users[0].hand > users[1].hand) {
					cout << users[0].userName << " Won!\n";
					users[0].score = users[0].bet + users[1].bet + users[0].score;
				}
				if (users[1].hand > users[0].hand) {
					cout << users[1].userName << " Won!\n";
					users[1].score = users[0].bet + users[1].bet + users[1].score;
				}

				scores(x, users);
				cout << "House's Hand: Score = " << scoreHand(HouseHand, HouseCardCount) << endl;
				displayHand(HouseHand, HouseCardCount);
				cout << "If the users hand is = to 0, the user has Busted!" << endl;
				cout << users[0].userName << "'s Hand: Score = " << users[0].hand << endl;
				cout << users[1].userName << "'s Hand: Score = " << users[1].hand << endl;

			}
			if (i == 2) {

				if (users[0].hand == users[1].hand) {
					cout << "Tie!\n";
					cout << "All gained $0\n" << endl;
					users[0].score = users[0].bet + users[0].score;
					users[1].score = users[1].bet + users[1].score;
					users[2].score = users[2].bet + users[2].score;
				}

				// Start user 0 check
				if ((users[0].hand > users[1].hand) && (users[0].hand > users[2].hand)) {
					cout << users[0].userName << " Won!\n";
					users[0].score = users[0].bet + users[1].bet + users[2].bet + users[0].score;
				}

				// Start user 1 check
				if ((users[1].hand > users[0].hand) && (users[1].hand > users[2].hand)) {
					cout << users[1].userName << " Won!\n";
					users[1].score = users[0].bet + users[1].bet + users[2].bet + users[1].score;
				}

				// Start user 2 check 
				if ((users[2].hand > users[0].hand) && (users[2].hand > users[1].hand)) {
					cout << users[0].userName << " Won!\n";
					users[2].score = users[0].bet + users[1].bet + users[2].bet + users[2].score;
				}


				scores(x, users);
				cout << "House's Hand: Score = " << scoreHand(HouseHand, HouseCardCount) << endl;
				displayHand(HouseHand, HouseCardCount);
				cout << "If the users hand is = to 0, the user has Busted!" << endl;
				cout << users[0].userName << "'s Hand: Score = " << users[0].hand << endl;
				cout << users[1].userName << "'s Hand: Score = " << users[1].hand << endl;
				cout << users[2].userName << "'s Hand: Score = " << users[2].hand << endl;

			}
		}
		else {
			system("cls");
			cout << "===========================================" << endl;
			cout << "=         Black Jack Round Finished       =" << endl;
			cout << "===========================================\n\n" << endl;
			if (i == 1) {
				if (users[0].hand == users[1].hand == HouseScore) {
					cout << "Tie!\n";
					cout << "You both gained $0\n" << endl;
					users[0].score = users[0].bet + users[0].score;
					users[1].score = users[1].bet + users[1].score;
				}
				// Start user 0 check
				if ((users[0].hand > users[1].hand) && (users[0].hand > HouseScore)) {
					cout << users[0].userName << " Won!\n";
					users[0].score = users[0].bet + users[1].bet + users[0].score;
				}

				// Start user 1 check
				if ((users[1].hand > users[0].hand) && (users[1].hand > HouseScore)) {
					cout << users[1].userName << " Won!\n";
					users[1].score = users[0].bet + users[1].bet + users[1].score;
				}

				// Start house check
				if ((HouseScore > users[0].hand) && (HouseScore > users[1].hand)) {
					cout << "The House Won! \nAll money bet for this hand is lost!\n" << endl;
				}

				scores(x, users);
				cout << "House's Hand: Score = " << scoreHand(HouseHand, HouseCardCount) << endl;
				displayHand(HouseHand, HouseCardCount);
				cout << "\nIf the users hand is = to 0, the user has Busted!" << endl;
				cout << users[0].userName << "'s Hand: Score = " << users[0].hand << endl;
				cout << users[1].userName << "'s Hand: Score = " << users[1].hand << endl;
			}

			if (i == 2) {
				if (users[0].hand == users[1].hand == HouseScore) {
					cout << "Tie!\n";
					cout << "You both gained $0\n" << endl;
					users[0].score = users[0].bet + users[0].score;
					users[1].score = users[1].bet + users[1].score;
				}
				// Start user 0 check
				if ((users[0].hand > users[1].hand) && (users[0].hand > users[2].hand) && ((users[0].hand > HouseScore))) {
					cout << users[0].userName << " Won!\n";
					users[0].score = users[0].bet + users[1].bet + users[2].bet + users[0].score;
				}

				// Start user 1 check
				if ((users[1].hand > users[0].hand) && (users[1].hand > users[2].hand) && ((users[1].hand > HouseScore))) {
					cout << users[1].userName << " Won!\n";
					users[1].score = users[0].bet + users[1].bet + users[2].bet + users[1].score;
				}

				// Start user 2 check 
				if ((users[2].hand > users[0].hand) && (users[2].hand > users[1].hand) && ((users[2].hand > HouseScore))) {
					cout << users[0].userName << " Won!\n";
					users[2].score = users[0].bet + users[1].bet + users[2].bet + users[2].score;
				}

				// Start house check
				if ((HouseScore > users[0].hand) && (HouseScore > users[1].hand) && ((HouseScore > users[2].hand))) {
					cout << "The House Won! \nAll money bet for this hand is lost!\n" << endl;
				}

				scores(x, users);
				cout << "House's Hand: Score = " << scoreHand(HouseHand, HouseCardCount) << endl;
				displayHand(HouseHand, HouseCardCount);
				cout << "If the users hand is = to 0, the user has Busted!\n" << endl;
				cout << users[0].userName << "'s Hand: Score = " << users[0].hand << endl;
				cout << users[1].userName << "'s Hand: Score = " << users[1].hand << endl;
				cout << users[2].userName << "'s Hand: Score = " << users[2].hand << endl;
			}
		}

		system("pause");
		system("cls");
		cout << "Would you like to play another game or end? yes(1) or no(2) " << endl;
		cin >> choice;
		while (!((choice == 1) || (choice == 2))) { // Validation if ented wrong item
			cout << "You entered the wrong choice.\nPlease enter again: ";
			cin >> choice;
		}

		if (choice == 2) {
			flag = false;
		}
		else {
			flag = true;
			system("cls");
		}
	}
}

//============================================
//				  shuffle()		             =
// This is where the deck is shuffled		 =
//============================================
void shuffle(bool CardsDealt[]) {

	for (int Index = 0; Index < 52; ++Index) {
		CardsDealt[Index] = false;
	}
}

//============================================
//				  displayCard()		         =
// This is where the cards are displayed     =
// to the user                               =
//============================================
void displayCard(int Card) {

	// Displays the Rank
	const int Rank = (Card % 13);

	if (Rank == 0) {
		cout << "Ace/";
	}
	else if (Rank < 9) {
		cout << (Rank + 1);
	}
	else if (Rank == 9) {
		cout << "Ten";
	}
	else if (Rank == 10) {
		cout << "Jack/";
	}
	else if (Rank == 11) {
		cout << "Queen/";
	}
	else {
		cout << "King/";
	}

	// Displays the Suit
	const int Suit = (Card / 13);

	if (Suit == 0) {
		cout << "Club";
	}
	else if (Suit == 1) {
		cout << "Diamond";
	}
	else if (Suit == 2) {
		cout << "Hart";
	}
	else {
		cout << "Spade";
	}
}

//=============================================
//				  displayHand()		          =
// This is were the whole hand is displayed   =
// this is different from displayCard because =
// it shows the whole hand not just one card  =
//=============================================
void displayHand(int Hand[], const int CardCount) {

	for (int CardIndex = 0; CardIndex < CardCount; ++CardIndex) {
		const int NextCard = Hand[CardIndex];
		displayCard(NextCard);
		cout << " ";
	}
	cout << endl;
}

//============================================
//				  nextCard()		         =
// A new card is delt to the user and the    =
// computer based on hit or who's turn       =
//============================================
int nextCard(bool CardsDealt[]) {

	bool CardDealt = true;
	int NewCard = -1;
	do {
		NewCard = (rand() % 52);
		if (!CardsDealt[NewCard]) {
			CardDealt = false;
		}
	} while (CardDealt);
	return NewCard;
}

//============================================
//				  scoreHand()		         =
// The scoreHand is the score for the hand   =
// If the user has 1 ace and 1 jack the		 =
// hand score will be 21                     =
//============================================
int scoreHand(int Hand[], const int CardCount) {

	int AceCount = 0;
	int Score = 0;
	for (int CardIndex = 0; CardIndex < CardCount; ++CardIndex) {
		const int NextCard = Hand[CardIndex];
		const int Rank = (NextCard % 13);
		if (Rank == 0) {
			++AceCount;
			++Score;
		}
		else if (Rank < 9) {
			Score = Score + (Rank + 1);
		}
		else {
			Score = Score + 10;
		}
	}
	while (AceCount > 0 && Score < 12) {
		--AceCount;
		Score = Score + 10;
	}
	return Score;
}

//============================================
//			dislpayScoresAndHands()		     =
// Shows the handScores from the user and    =
// the computer which will take place        =
// once the game is won                      =
//============================================
void displayScoresAndHands(int HouseHand[], const int HouseCardCount, int PlayerHand[], const int PlayerCardCount) {

	cout << "House's Hand: Score = " << scoreHand(HouseHand, HouseCardCount) << endl;
	displayHand(HouseHand, HouseCardCount);
	cout << "\nPlayer's Hand: Score = " << scoreHand(PlayerHand, PlayerCardCount) << endl;
	displayHand(PlayerHand, PlayerCardCount);
	cout << endl;
}

//============================================
//				  score()		             =
// The score is the score for all the money  =
// bet in the all the money bet in the       =
// game for both computer vs user and		 =
// user/user vs computer or the house		 =
//============================================
void scores(int x, Player users[])
{
	// Declarations
	fstream inScores;
	string line;
	string userName;
	int score;
	bool found = false;

	userName = users[x].userName;
	score = users[x].score;

	inScores.open("Scores.dat", ios::in | ios::out | ios::beg); // Stores the username and score
		while (getline(inScores, line) && !found) {
			if (line.compare(userName) == 0) { //match strings exactly!
				found = true; // found is true => break loop
				inScores << score;
			}
		}
	inScores.close();
}