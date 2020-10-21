// ParcelPals.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <locale>
#include <random>
#include <chrono>
#include <ctime> 
using namespace std;


vector<int> playersMoney;//money of all players
vector<int> playersSpace;//spaces of all players
vector<vector<string>> playersDeliveries;//deliveries of all players
//index 0 of first vector will be player 1. the vector at index 0 will be the strings of the names of the residences
//that player has to deliver to



int chanceSpaces[] = {10,19,28};
int postOffices[] = { 3, 8, 12, 17, 21, 26, 30, 35 };
int mansions[] = { 2, 9, 11, 18, 20,27,29,36 };
int houses[] = { 4,5,6,7,22,23,24,25 };
int apartments[] = { 13,14,15,16,31,32,33,34 };
string residences[] = {"Harfield","Estrada", "Mackenzie", "Wolfram", "Cabera", "Lowery", "Fischer", "Thatcher", "Kelley", "Shapard", 
"Atkins", "Rodrigues", "Howells", "Barlow"};

int playerTurn = 0;//this is the players turn - 1. player1 is 0, player4 is 3

bool allDelivered = false;//true when all packages are delivered

locale loc;

//declare
bool isHouse(int);
bool isMansion(int);
bool isApartment(int);
string getHouseName(int);
string getMansionName(int);
string getApartmentName(int);
bool givePlayerDelivery(int, bool);
int RNG(int, int);


int main()
{
	srand(time(NULL));//make it truely random
	int playerCount;
	while (true) {
		playerCount = NULL;
		cout << "How many players will be playing the game?\n";
		cin >> playerCount;
		if (cin.good()) {
			if (playerCount >= 2 && playerCount <= 4) {//right amout of players (2-4)
				break;
			}
			else
			{
				cout << "There can only be 1-4 players!\n";
				cin.clear();
				cin.ignore(1, '\n');
			}
		}
		else
		{
			cout << "Invalid input, try again.\n";
			cin.clear();
			cin.ignore(1, '\n');
		}
	}
	//index 0 of the array will be player 1, index one of the array will be player 2
	vector<int> money(playerCount, 200);//set each players' money to 200
	playersMoney = money;//cause you cant split vector declaration and initialization
	
	vector<int> space(playerCount , 1);//set each players' space to 1 (spaces will be 1 indexed)
	//index 1 of the array will be the first space, index 2 will be the second space.
	playersSpace = space;//cause you cant split vector declaration and initialization

	
	


	
	while (!allDelivered) {
		int timesRolled = 1;//how many times to roll the dice

		while (true) {

			std::string response;
			cout << "Player " << playerTurn + 1 << ", what would you like to do?\n";
			//player can either roll, see deliveries, or use a card(if they have any)
			cin >> response;
			if (cin.good()) {

				if (response.find("roll") != string::npos) {//if response contains the word "roll"
					break;
				}//more to be added (as an else)! XX
				
			}
			else
			{
				cout << "Invalid input. Try again. Your input must contain what you want to do within the sentence!\n";
				cin.clear();
				cin.ignore(1, '\n');
			}
		}
		int diceRoll = 0;
		for (int i = 0; i < timesRolled; i++) {
			diceRoll += RNG(1, 6);
		}
		
		
		cout << "You rolled a total of " << diceRoll << "!\n";
		
		playersSpace[playerTurn] += diceRoll;
		if (playersSpace[playerTurn] > 36) {
			playersSpace[playerTurn] = playersSpace[playerTurn] - 36;//set you to the start of the board. start space is 1
		}
		int spaceOn = playersSpace[playerTurn];//space current player is on

		//check for space player is on
		if (isHouse(spaceOn)) {
			cout << "You have landed on the " + getHouseName(spaceOn) << " residence!\n";
		}
		if (isMansion(spaceOn)) {
			cout << "You have landed on the " + getMansionName(spaceOn) << " residence!\n";
		}
		if (isApartment(spaceOn)) {
			cout << "You have landed on the " + getApartmentName(spaceOn) << " residence!\n";
		}
		//check if player is on chance space
		if (find(begin(chanceSpaces),end(chanceSpaces),spaceOn) != end(chanceSpaces)) {
			cout << "You have landed on a chance square! You drew a card:\n";
			givePlayerDelivery(playerTurn, false);
			cout << playersDeliveries[playerTurn][0];
		}
		//check if player is on post office
		if (find(begin(postOffices), end(postOffices), spaceOn) != end(postOffices)) {
			cout << "You have landed on a post office square!\n";
			givePlayerDelivery(playerTurn, false);
			cout << playersDeliveries[playerTurn][0];

		}


		if (playerTurn + 1 <= playerCount-1) {//get next player's turn
			playerTurn++;
		}
		else
		{
			playerTurn = 0;
		}
	}

}

bool isHouse(int i) {//check if the square is a house or not
	for (int u : houses) {
		if (u == i) {
			return true;
		}
	}
	return false;
}
bool isMansion(int i) {//check if the square is a mansion or not
	for (int u : mansions) {
		if (u == i) {
			return true;
		}
	}
	return false;
}
bool isApartment(int i) {//check if the square is an apartment or not
	for (int u : apartments) {
		if (u == i) {
			return true;
		}
	}
	return false;
}

string getHouseName(int i)
{
	if (i == 4 || i == 5) {
		return "Estrada";
	}
	else if (i == 6 || i == 7) {
		return "Mackenzie";
	}
	else if (i == 22 || i == 23) {
		return "Kelley";
	}
	else if (i == 24 || i == 25) {
		return "Shepard";
	}
}

string getMansionName(int i)
{
	if (i == 2) {
		return "Harfield";
	}
	else if (i == 9) {
		return "Wolfram";
	}
	else if (i == 11) {
		return "Cabera";
	}
	else if (i == 18) {
		return "Fischer";
	}
	else if (i == 20) {
		return "Thatcher";
	}
	else if (i == 27) {
		return "Atkins";
	}
	else if (i == 29) {
		return "Rodrigues";
	}
	else if (i == 36) {
		return "Barlow";
	}
	
}

string getApartmentName(int i)
{
	if (i == 13 || i == 14 || i == 15 || i == 16) {
		return "Lowery";
	}
	else if (i == 31 || i == 32 || i == 33 || i == 34) {
		return "Howell";
	}
}

bool givePlayerDelivery(int player, bool goOver) {
	if (playersDeliveries[player].size() <= 5 || goOver) {//has less than 5 deliveries or they draw the card to get an extra delivery
		int deliveryRes;
		do
		{
			
			deliveryRes = RNG(0, residences->size() - 1);//get random index of residencs
			
			//cout << deliveryRes;
			
		} while (!residences[deliveryRes].compare("TAKEN"));//a bit of inefficient code, but it gets the job done
		
			string indexOfRes = residences[deliveryRes];
			vector<string> temp = playersDeliveries[player];
			//temp.insert();

			//temp[].append(indexOfRes);
			//playersDeliveries[player] = temp;
		//playersDeliveries[player].push_back(playersDeliveries[player][playersDeliveries[player].size()].push_back(indexOfRes));
		//playersDeliveries[player][playersDeliveries[player].size()] = residences[deliveryRes];//set player delivery to that residence
		residences[deliveryRes] = "TAKEN";//set to "TAKEN"
		return true;
	}
	else
	{
		return false;
	}
}

int RNG(int low, int high) {
	return rand() % high + low;//return random number
}
