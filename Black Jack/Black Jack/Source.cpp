/**
	Game of Blackjack
	You can play with one or two players
	each player creaates a account if they don't have one and if they do they can load their existing data
	Each player and the dealer are dealt two cards, the dealers first card is face down
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

//enum types for the card suits
enum card_suit{
	S, H, D, C
};

//Global Variables - yes I know they are bad but rather than pass this everywhere I put it here. Also allows me to change the min bet in one spot rather than everywhere
int MIN_BET = 5; //Minimum bet is $5

//Structures
struct account{ //account structure. This will store all the player information
	string name; //The name on the account
	string email; //the email address registered to this account
	string username; //the username of this account
	string password; //pasword for the account

	int money; //in case they decide to quit playing when they come back their money will be there may not use this but we'll see

	
	int wins; //games stats
	int total_played; //game stats
	double win_percentage; //game stats
};

struct card{ //how the cards are stored
	int value; //value of the card
	card_suit suit; //suit of the card we have. Not really that important in this game but might as well have it
	bool up; //for the dealer since one card is face down
};

struct player{ //basis of each player in the game including the dealer
	account info; //player's account info
	vector<card> hand; //the players hand
	int bet; //how much the user is betting
	int insurance_bet; //if they take the insurance
};

//prototypes
card deal();
card hitMe();
int score(vector<card>);
bool hasAce(vector<card>);
void printCards(vector<card>);
char printSuit(card);
void doubleDown(player, player&);
int winner(player, player&);
void payout(player, player&);
void split(player&, player&);
void bet(player&);
void insurance(vector<player>&);
account load();
void save(player);
account create();
void play(vector<player>&);
void clear(vector<card>&);
void dealer_play(player&);
void displayMenu();
void displayRules();

int main(){
	unsigned int time_seed = time(0); //so we can get a random number
	srand(time_seed); //seeding the random number
	player user; //creates a player called user
	player dealer; //creates the dealer
	vector<player> players; //creates a vector of players
	char input; //for the Input

	dealer.info.username = "Dealer"; //sets up the dealer info
	players.push_back(dealer); //puts the dealer in the vector

	int menu; // so we can take in input

	do{
		displayMenu(); //displays the menu
		cin >> menu; //take in the user input

		if(menu == 1){ //if the user wants to read the rules
			displayRules(); //displays the rules
		}
		else if(menu == 2){ //if the user wants to play
				cout << "How many players(1 or 2)" << endl; //asks how many players
				int numPlayers; //so we can take in how many players
				cin >> numPlayers; //takes in the input
				for(int i = 0;i< numPlayers;i++){ //cycles through the players
					cout << "Are you a new player(N) or existing player(E)?" << endl; //asks if they are a new or existig player
					cin >> input; //takes in their input

					if(input == 'N' || input == 'n'){ //if they are new
						user.info = create(); //creates new account info
						save(user); //saves the player
						players.push_back(user); //pushes the player on the vector
					}
					else if(input == 'E' || input == 'e'){ //if they are a existing player
						user.info = load(); //loads their account info
						players.push_back(user); //pushes them on the stack
					}
			
				}
		}
		else{ //if the user can't follow directions
			cout << "Please enter 1 or 2" << endl; //reiterates their only options
			menu = 0; //resets menu
			cin.ignore(); //ignores what's in the buffer
		}
	}while(menu != 2); //while they don't choose play we will repeat this until we finish the never ending story
	
	bool cond = false; //so we can ask if they want to continue
	int size = players.size();
	do{
		for(int i =1;i<players.size();i++){
			if(players[i].info.money < 5){ //checks to see if they player has any money if not they can add more
				cout << players[i].info.username << " is currently out of money. Would you like to add some, yes(Y) or (N)?" << endl; //Lets them know they are out of funds and if they want to add some
				cin >> input; //takes in input
				if(input == 'Y' || input == 'y'){ //if they say yes
					cout << "How much would you like to add?" << endl; //how much
					cin >> players[i].info.money; //takes it in
				}
				else{ //if not we save their info and leave
					save(players[i]); //saves info
					players.erase(players.begin() + (i)); //erases player from vector
					i--;

				}
			}

			if(cond && players.size() > 1){//if they have played at least once
				cout << players[i].info.username << " Would you like to keep playing(P) or quit(Q)" << endl;
				cin >> input; //takes in input
				if(input == 'q' || input == 'Q'){ //player want to quit
					save(players[i]); //saves info
					players.erase(players.begin() + (i)); //erases player from vector
					i--;
				}
			}
		}
		play(players); //and we play
		cond = true; //they finished one hand
	}while(players.size() > 1);
	//!(input == 'q' || input == 'Q')

	return 0;
}
/**
	Simple method that displays the rules of the game only
*/
void displayRules(){
	cout << "The rules of blackjack are fairly simple and your only opponent in the game is the dealer. Each player is dealt a two cards face up while the dealer only has one card face up. The goal is to have your two cards added total to be higher than the dealer’s two cards and under 21. If you go over 21 you “bust”, or lose, or if the dealers two cards added together are higher than yours you also lose.	 If your two card total is equal to the dealers then it is a “stand-off” or a tie. Cards values are usually face value, 2 is 2 and 10 is 10, while all face cards, jack/queen/king, are also 10. The only exception to this rule are Aces, they can take on the value of 1 or 11. To get your two cards total you simply add your two cards together. If you have any combination of an Ace or any card that is 10 points then you have what is called blackjack, 21 pts. Getting blackjack means you get paid more if you win. With all of that being said if you’re not satisfied with your two card total then you can take extra cards, called taking a hit, and for each hit you get you get one more card. The dealer does this as well but has a strict set of rules to follow on whether or not to take a hit. If a dealer’s hand is less than or equal to 16 or a “soft” 17, meaning they have an ace and a 6, they must take a hit. If it’s 17 or higher the dealer must stand.  So now that we know the dealer rules there are a few options for users.  As I stated earlier you can take hits to increase your card total. You may also split your cards and double down. Splitting can be done when your first two cards are of equal value and can only be split from the original hand, split hands cannot be split, the bet has to be equal or greater than the original bet for each hand. For each time you split you will receive and additional card for that hand and then you play like regular blackjack.  Users may also double down which consists of a user placing another bet of equal or lesser value when their first two cards total is equal to 9, 10, or 11 without aces. After doubling down you will only get one additional card. Some of you may have realized that if the dealer gets a blackjack or 21 you pretty much always lose, unless you yourself have blackjack. There is a way around this and it’s called insurance. If the dealer is dealt an ace face up then the dealer will ask if you want to take out insurance, equal to half of your original bet, to insure your hand if the dealer has blackjack and only when he has blackjack and helps insure you don’t lose money if he does have blackjack, insurance pays 2 to 1 so your insurance bet will cover the loss of your hand if you bet half." << endl; //displays the rules
}
/**
	Simple method that displays the menu for the game only
*/
void displayMenu(){
	cout << "Welcome to the game of Blackjack!" << endl; //output
	cout << "Please select an option below." << endl; //output
	cout << "1) Rules" << endl; //output
	cout << "2) Play" << endl; //output
}
/**
	Rules for the dealer. They hit on everything up to 17 including a soft 17
*/
void dealer_play(player &dealer){
	if((score(dealer.hand) < 17) || (score(dealer.hand) == 17 && hasAce(dealer.hand))){ //dealer hits at less than 17 and on soft 17
		dealer.hand.push_back(hitMe()); //dealer gets a card
	}	
}
/**
 how we will play
 This method takes care of all the play portions of the game. Taking hits, splitting, double down, etc
 Takes in the vector of players by reference so we can make changes directly to the players
*/
void play(vector<player> &players){
	char input; //for our input

	for(int i =1;i<players.size();i++){ //cycles through the players
		bet(players[i]); //players bet here
		cout << "Money: " << players[i].info.money << setw(10) << "Bet: " << players[1].bet << endl; //display their money and bet amount
	}
	
	/**the below function goes through each player and deals them two cards. To make it more realistic
		I actually went through every player and gave them one card a time. So it starts with the dealer
	*/
	for(int i = 0;i< (players.size()*2);i++){ 
		players[(i%players.size())].hand.push_back(deal());
		if((i%players.size()) == 0 && (i%2) == 0){ //the dealers first card
			players[(i%players.size())].hand[(i%2)].up = false; //is set to false since it's face down
		}
	}
	/**
		The below function shows each players score but the dealers
	*/
	for(int i=1;i<players.size();i++){
		cout << players[i].info.username << " has: " << score(players[i].hand) << setw(10) << endl;
	}

	/**
		The below function displays each persons cards
	*/
	for(int i =0;i<players.size();i++){
		cout << players[i].info.username << " Cards:" << endl;
		printCards(players[i].hand);
	}
	
	/**
		The below function  actually consists of the playing
	*/
	bool cont = true; //we will use this
	for(int i = 1;i<players.size();i++){ //we cycle through all the players since each player plays on their own
		do{
			if(players[0].hand[1].value == 1 && cont){ //if the dealer has an ace and cont is true - basically this only happens the first time if the dealer doesn't have blackjack
				insurance(players); //asks player if they want insurance
				if(score(players[0].hand) == 21){ //checks to see if the dealer has blackjack - we know the first card is an A
					players[0].hand[0].up = true; //if they do we set the first card to face up
			
					printCards(players[0].hand); //prints the dealers cards
					
					/**
						The below function cycles through the players and pays them out since they lost
					*/
					for(int i =1;i<players.size();i++){
						payout(players[0],players[i]);
					}
					input = 'S'; //sets input to stay since they just lost
				}
				cont = false; //if the dealer didn't have blackjack this is now false
			}
			if(players[0].hand[1].value >= 10 && cont){ //if the dealer has a 10 or face card showing we don't check for insurance but if they have blackjack that's game
				if(score(players[0].hand) == 21){ //if they have blackjack
					players[0].hand[0].up = true; //puts the dealers first card face up

					printCards(players[0].hand); //prints the dealers card
					/**
						The below function pays out the players since they just lost
					*/
					for(int i =1;i<players.size();i++){
						payout(players[0],players[i]);
					}
					input = 'S'; //input is now S since the players lost					
				}			
				cont = false; //if the dealer doesn't have 21 we don't care about this anymore
			}
			/**
				As long as the players score is less than 21
			*/
			if(score(players[0].hand) <= 21){
					/**
					The most complicated first - if they have a pair of 5's they can split, double down, hit, or stay
					*/
					if(((players[i].hand[0].value >= 10 && players[i].hand[1].value >= 10) || players[i].hand[0].value == players[i].hand[1].value) && players[i].hand.size() == 2  && score(players[i].hand) == 10){ 
						cout << players[i].info.username << " score: " << score(players[i].hand) << endl; //shows them their score
						cout << "Would you like to Double Down(D), split(L),take a hit(H), or stay(S), default is to take a stay?" << endl; //ask them
					}
					else if(((players[i].hand[0].value >= 10 && players[i].hand[1].value >= 10) || (players[i].hand[0].value == players[i].hand[1].value)) && players[i].hand.size() == 2){ //if they can split their cards
						cout << players[i].info.username << " score: " << score(players[i].hand) << endl; //shows them their score
						cout << "Would you like to split(L) your cards, take a hit(H), or stay(S), default is to take a stay?" << endl; //ask them
					}
					else if(players[i].hand.size() == 2 && score(players[i].hand) >= 9 && score(players[i].hand) <= 11 && !(hasAce(players[i].hand))){ //can they double down
						cout << players[i].info.username << " score: " << score(players[i].hand) << endl; //shows them their score
						cout << "Would you like to Double Down(D), take a hit(H), or stay(S), default is to take a stay?" << endl; //asks them
					}
					else{ //they can't do anything special
						cout << players[i].info.username << " score: " << score(players[i].hand) << endl; //shows them their score
						cout << "Hit(H) or Stay(S), default is to take a stay?"; //asks them what they want to do
					}
					cin >> input; //takes in the input
					switch(input){ //what did they choose?
					case 'L': //they wanted to split
						split(players[0], players[i]); //we split them
						printCards(players[i].hand); //reprint their cards in case they forgot
						break; 
					case 'D':
						doubleDown(players[0], players[i]); //they double down
						input = 'S'; //sets input to S since now they are done
						break;
					case 'H':
						players[i].hand.push_back(hitMe()); //we give them one more card for their hit
						printCards(players[i].hand); //reprint their cards
						cout << players[i].info.username << " score is now " << score(players[i].hand) << endl; //reprint their score
						break;
					default: //this is here for people can't follow directions
						input = 's'; //input is S
					}
					if(score(players[i].hand) > 21){ //if they bust they are done
						input = 'S'; //so we can quit
					}
			}
		}while(!(input == 'S' || input == 's')); //we continue doing this until they are want to stay
	}

	dealer_play(players[0]); //now the dealer plays

	players[0].hand[0].up = true; //now the everybody can see the first card
	
	/**
		The below method shows everybody's score and cards including dealers
	*/
	for(int i =0;i<players.size();i++){
		cout << players[i].info.username << " score: " << score(players[i].hand) << " Cards: ";
		printCards(players[i].hand);
	}

	/**
		The below method pays everybody out
	*/
	for(int i =1;i<players.size();i++){
		if(score(players[i].hand) > 21){ //if the player busted we tell them
			cout << "You busted! ";
		}
		int win = winner(players[0], players[i]); //we figure out who wins
		if(win == 1){
			players[i].info.wins += 1; //if the player wins we add one to their win record
		}
		payout(players[0],players[i]); //we payout everybody
		clear(players[i].hand); //we clear out their hands
		players[i].info.total_played+=1; //adds one to the total played	
	}	

	clear(players[0].hand); //clear out the dealers hand
}
/**
	The below method clears out a players hand
*/
void clear(vector<card> &hand){
	hand.clear();
}
/**
Creates new accounts for new players
*/
account create(){
	account user; //creates a new account
	cout << "What is the username you'd like to use?" << endl;
	fstream input; //so we can take in the input
	string filename; //what file we will open

	/**
		The below method basically checks to see if a username is available by checking to see if the file can even be open
	*/
	do{
		cin >> user.username; //takes in the username
		filename = user.username + ".dat"; //adds the extension to the file
		input.open(filename); //opens the file
		if(!input.fail()){ //if it didn't fail the name is taken
			cout << "This username is already taken. Please choose another one." << endl; //we tell the user
		}
	}while(!input.fail()); //repeat until it doesn't fail

	cout << "Please enter a password." << endl; //asks for password
	cin.ignore(); // so cin will work
	getline(cin, user.password); //takes in the line b/c it may be more than one word
	cin.ignore(); //so the next line will wokr

	cout << "Please enter your name." << endl; //asks for the users name
	getline(cin, user.name); //takes it in

	cout << "Please enter your email address." << endl; //asks for email address
	cin >> user.email; //takes it in

	cout << "Please enter how much money you'd like to deposit" << endl; //asks for money
	cin >> user.money; //takes it in

	user.total_played = 0; //sets these to 0
	user.wins = 0; //sets these to 0
	user.win_percentage = 0; //set these to 0

	input.close(); //closes the input stream
	return user; //returns the account
}
/**
	For saving account info for when you're done
*/
void save(player user){
	ofstream output; //output stream
	string filename = user.info.username + ".dat"; //so we know what file to save to

	output.open(filename, ios::out); //we want to erase all previous data

	if(!output){//if we can't open output
		cerr << "Output file will not open and data could not be saved. " << endl;
	}

	double percent = 0; //for the win percentage
	if(user.info.total_played == 0){
		percent = 0; //so we don't divide by zero
	}
	else{
		percent = ((double)user.info.wins/user.info.total_played)*100; //win percentage
	}
	output << user.info.username << endl << user.info.password << endl << user.info.name << endl << user.info.email << endl << user.info.money << endl << user.info.total_played << endl << user.info.wins << endl << setprecision (3) << percent; //writes the data to the file

	output.close(); //closes the output stream
}

/**
For loading account info
*/
account load(){
	string username, password; //so we know what the username and password is
	string filename; //how we will save the data
	account user; //sets up an account so we can take in the data
	ifstream input; //file stream 

	/**
		We get the users username and password
	*/
	do{
		cout << "What is your username?" << endl; //asks for username
		cin >> username; //take it in
		filename = username + ".dat"; //append the file extension
		input.open(filename); //opens the file //opens the file
	}while(input.fail()); //if it fails to open they probably put in the wrong username so we keep doing this until they get it right

	getline(input, user.username); //take in the username
	getline(input, user.password); //take in the password

	do{
		cout << "What is your password?" << endl; //asks for the password
		cin >> password; //takes it in
	}while(!(password == user.password)); //checks password and keeps checking until the user gets it right
	
	/**
		Takes in all the user info 
	*/
	getline(input, user.name);
	getline(input, user.email);
	input >> user.money;
	input >> user.total_played;
	input >>user.wins;
	input >>user.win_percentage;
	
	input.close(); //close out the file
	return user; //returns the account info
}
/**
	if the dealer is displaying an Ace it asks all the players if they want to take out insurance
*/
void insurance(vector<player> &players){
	int size = players.size(); //gets the size of the vector
	
	for(int i=1;i<size;i++){
		int bet; 
		players[i].insurance_bet = 0;
		if((players[i].info.money-(players[i].bet/2)) >=0){ //can the player even make an insurance bet
			do{
				cin.ignore();
				cout << "How much would you like to bet for insurance, up to " << (players[i].bet/2) << players[i].info.username << "?(0 if you don't want to take out insurance)" << endl; //asks if they want to make an insurance bet
				cin >> bet; //takes it in
			}while(!(bet <= (players[i].bet/2) && bet >=0)); //continues to do this until they input the right stuff

			if(bet != 0){ //did they make a insurance bet
				players[i].insurance_bet = bet; //we have the insurance bet set
				players[i].info.money-=bet; //we take it out of their money pile
			}
		}
	}

	if(score(players[0].hand) == 21){ //the dealer has 21
		for(int i = 1; i<size;i++){
			if(players[i].insurance_bet != 0){ //if they took out insurance
				players[i].info.money+=(players[i].insurance_bet*2); //pays out 2 to 1
				players[i].insurance_bet = 0; //clears out the insurance bet
			}
		}
	}
	else{
		for(int i = 1; i<size;i++){
			if(players[i].insurance_bet != 0){ //if they took out insurance
				players[i].insurance_bet = 0; //clears out the insurance bet
			}
		}
	}	
}

/**
   Asks the user what they would like to bet and returns it
*/
void bet(player &user){
	int bet;
	do{
		cout << user.info.username << endl; //so we know what player is betting
		cout << "How much would you like to bet? (Must be greater than the " << MIN_BET <<  " and less than " << user.info.money << ")" << endl; //we tell them what they can bet
		cin >> bet; //takes in their bet
	}while(!(bet >= MIN_BET && bet <= user.info.money)); //repeat until they get it right
	user.info.money -= bet; //subtract the bet from their money stock pile
	user.bet = bet; //set their bet
}
/**
	If the user chooses to split their cards
	The split hand is completely played here
*/
void split(player &dealer, player &user){
	player split;
	vector<player> players; //creates a new player for the split
	players.push_back(dealer);
	
	split.bet = user.bet; // takes the extra bet
	user.info.money-= user.bet; //takes out the bet from the user money pile
	split.hand.push_back(user.hand[0]); //takes the first card from the user and gives it to the split player
	split.hand.push_back(deal()); //gives the split person a new card
	user.hand[0] = deal(); //gives the user a new card
	split.info.username = user.info.username;
	players.push_back(split); //pushes the split player onto the vect

	printCards(players[1].hand); //prints out the new cards

	char input; //what the input is
	do{
		for(int i =1;i<players.size();i++){
			if(score(players[i].hand) > 21){ //if they bust
				input = 'S'; //they are done
			}
			else{
				cout << "Hit(H) or Stay(S):"; //otherwise we ask them if they want to take a hit
				cin >> input; //take in their input
			}
			/**
				If they take a hit the below code gives them a new card, prints out their new cards and their new score
			*/
			if(input == 'H' || input == 'h'){
				players[i].hand.push_back(hitMe());
				printCards(players[i].hand);
				cout << players[i].info.username << " score is now " << score(players[i].hand) << endl;							
			}
		}
	}while(!(input == 'S' || input == 's')); //we repeat this until they stop taking hits or bust

	dealer_play(players[0]); //dealer plays now since we need to take care of this now rather than later

	/**
		prints the split players cards and score again
	*/
	for(int i =1;i<players.size();i++){
		cout << players[i].info.username << " score: " << score(players[i].hand) << " Cards:" << endl;
		printCards(players[i].hand);
	}

	/**
		payouts the split player
	*/
	for(int i =1;i<players.size();i++){
		if(score(players[i].hand) > 21){
			cout << "You busted!"; //tells them they busted
		}
		payout(players[0],players[i]); //plays out the player
	}	
}
/**
	Typical pay out rules. If the player wins he gets 2 to 1 odds.
*/
void payout(player dealer, player &user){
	if(winner(dealer, user) == 1){ //if the player won
		if(score(user.hand) == 21 && hasAce(user.hand) && user.hand.size() == 2){ //if the player has blackjack it's a 3:2 payout
			user.info.money += ((user.bet*3)/2); //I think this is 3:2 odds
			user.bet = 0; //clears out the bet
			cout << user.info.username << " won!" << endl;
		}
		else{
			user.info.money+= (user.bet*2); //adds the bet to the players stash of cash
			user.bet = 0; //clears out the bet
			cout << user.info.username << " won!" << endl;
		}
	}
	else if (winner(dealer, user) == 0){ //they tied
		user.info.money+= user.bet; //players money goes back to his pile
		user.bet = 0; //clears out the bet
		cout << user.info.username << " tied!" << endl;
	}
	else{ //the dealer won.
		user.bet = 0; //player didn't win so all we need to do is clear out. 
		cout <<user.info.username << " lost!" << endl;
	}
}
/**
	Figures out if the dealer or the user won
	returns 1 if the player won
	returns -1 if the dealer won
	returns 0 if they tie
*/
int winner(player dealer, player &user){
	if(score(dealer.hand) == score(user.hand)){ //they had a stand off
		return 0;
	}
	else if(((score(dealer.hand) < score(user.hand)) && (score(user.hand) <= 21)) || (score(dealer.hand) > 21 && score(user.hand) <= 21)){ //user won
		return 1;
	}
	else{ //dealer typically wins
		return -1;
	}
}
/**
	Double down
	If the user chooses to double down we take in the new amount betted, deal one more card, and figure out if the user won
	takes in the user and dealer players and returns nothing
*/
void doubleDown(player dealer, player &user){
	int bet; //so we can store the new bet
	do{
		cout << "How much would you like to bet? (Must be greater than the " << MIN_BET << " and less than "<< user.info.money <<")" << endl; //asks how much they want to bet
		cin >> bet; //takes it in
	}while(!(bet > MIN_BET && bet <= user.bet && bet <= user.info.money)); //we repeat this until they get it right

	user.bet+=bet; //add the new bet to the original
	user.info.money-=bet; //takes out their double down bet from the money
	user.hand.push_back(deal()); //gives the user one more card
	payout(dealer, user); //pays out the player based on if he/she won		
}
/**
Returns the Character version of the suit
Fairly self-explanatory...takes in a number between 0-4 and returns the suit based off that number
*/
char printSuit(card new_card){
	switch(new_card.suit){ 
		case 0:
			return 'S';
		case 1:
			return 'H';
		case 2:
			return 'D';
		case 3:
			return 'C';
	}
}

/**
Prints the cards to the screen
*/
void printCards(vector<card> hand){
	const string CARD_VALUES[14] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "X"}; //makes it easier to print

	for(int i=0;i<hand.size();i++){
		/**cout << "----------" <<  endl
			<< "|" << ((hand[i].up)?(hand[i].value):('X')) << ((hand[i].up)?((printSuit(hand[i]))):('X')) << setw(6) << "|" << endl
				<< "|" << setw(8) << "|" << endl
				<< "|" << setw(8) << "|" << endl
				<< "|" << setw(8) << "|" << endl
				<< "|" << setw(6) << ((hand[i].up)?(hand[i].value):('X')) <<  ((hand[i].up)?(printSuit(hand[i])):('X')) << "|" << endl
				<< "----------" << endl;*/
		if(hand[i].up){ //if the hand is face up we print this
			cout << CARD_VALUES[(hand[i].value-1)] << printSuit(hand[i]) << " "; 
		}
		else{ //if it's face down we print XX
			cout << CARD_VALUES[13] << CARD_VALUES[13] << " ";
		}
	}
	cout << endl;
}
/**
	Lets us know if the hand has an ace
*/
bool hasAce(vector<card> hand){
	bool has_ace = false; //For now we say there is no ace in the hand
	for(int i =0;i<hand.size();i++){
		if(hand[i].value == 1){ //we have an ace
			has_ace = true; //so we set this to true
		}
	}

	return has_ace;
}
/**
	Gets the score for the user
	We treat Aces initially as 1 and then later check to see if the hand contains an Ace
*/
int score(vector<card> hand){
	int total = 0; //setting up the total value
	for(int i = 0; i<hand.size();i++){
		if(hand[i].value >= 10){ //if it's 10, J, Q, or K
			total+=10; //adds 10 to the toal
		}
		else{
			total += hand[i].value; //adds the value to the total 
		}
	}

	if(hasAce(hand) && total <= 11){ //if the hand has an ace and we won't bust
		total+=10; //add 10
	}

	return total; //return the total
}
//gets a new card for the player
card hitMe(){
	return deal(); //add another card to the players hand
}
/**
	takes in nothing and returns a card
	makes a new card and assigns it a random value between 1-13
	assigns a suit to the card as well
	returns the card
*/
card deal(){
	card new_card; //card we will be returning

	new_card.value = 1 + rand() % 13; //makes sure the random number is between 1 and 13 for the value
	int suit = rand() % 4;
	switch(suit){ //makes sure the random number is between 1 and 4 for the suit
		case 0:
			new_card.suit = S;
			break;
		case 1:
			new_card.suit = H;
			break;
		case 2:
			new_card.suit = D;
			break;
		case 3:
			new_card.suit = C;
			break;
	}

	new_card.up = true; //we'll change it later if it's the dealers face down card

	return new_card; //returning the card
}
