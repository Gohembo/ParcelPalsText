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


int chanceSpaces[] = {10,19,28};
int postOffices[] = { 3, 8, 12, 17, 21, 26, 30, 35 };
int mansions[] = { 2, 9, 11, 18, 20,27,29,36 };
int houses[] = { 4,5,6,7,22,23,24,25 };
int apartments[] = { 13,14,15,16,31,32,33,34 };

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


int main()
{
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
			
			srand(time(NULL));//make it truely random
			diceRoll += rand() % 6 + 1;//add to diceroll
		}
		
		
		cout << "You rolled a total of " << diceRoll << "!\n";
		
		playersSpace[playerTurn] += diceRoll;
		if (playersSpace[playerTurn] > 36) {
			playersSpace[playerTurn] = playersSpace[playerTurn] - 36;//set you to the start of the board. start space is 1
		}
		int spaceOn = playersSpace[playerTurn];//space current player is on

		if (isHouse(spaceOn)) {
			cout << "H";
		}
		if (isMansion(spaceOn)) {
			cout << "M";

		}
		if (isApartment(spaceOn)) {
			cout << "A";

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

string getHouseName(int)
{
	return string();
}

string getMansionName(int)
{
	return string();
}

string getApartmentName(int)
{
	return string();
}
