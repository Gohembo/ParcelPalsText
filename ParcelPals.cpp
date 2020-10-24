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
#include <algorithm>
#include <cstdlib>
using namespace std;



vector<int> playersMoney;//money of all players
vector<int> playersSpace;//spaces of all players
vector<vector<string>> playersDeliveries{ {"","","","","",""},{"","","","","",""},{"","","","","",""},{"","","","","",""}};//deliveries of all players
//index 0 of first vector will be player 1. the vector at index 0 will be the strings of the names of the residences
//that player has to deliver to
vector<vector<string>> playersEvents{ {"","","","","","","","","",""},{"","","","","","","","","",""},{"","","","","","","","","",""},{"","","","","","","","","",""} };
vector<bool> porchBandit;
vector<bool> extraPackage;
vector<int> extraRolls;
vector<string> playerNames;

int chanceSpaces[] = {10,19,28};
int postOffices[] = { 3, 8, 12, 17, 21, 26, 30, 35 };
int mansions[] = { 2, 9, 11, 18, 20,27,29,36 };
int houses[] = { 4,5,6,7,22,23,24,25 };
int apartments[] = { 13,14,15,16,31,32,33,34 };
string residences[] = { "Lowery","Howells","Barlow","Fischer","Thatcher","Atkins","Rodrigues","Harfield","Wolfram","Cabera","Mackenzie","Estrada","Shepard","Kelley" };

int playerTurn = 0;//this is the players turn - 1. player1 is 0, player4 is 3

bool allDelivered = false;//true when all packages are delivered

int deliveryIndex = 0;

locale loc;
static unsigned int g_seed;//fastrand
//declare
bool isHouse(int);
bool isMansion(int);
bool isApartment(int);
string getHouseName(int);
string getMansionName(int);
string getApartmentName(int);
bool givePlayerDelivery(int, bool);
int RNG(int, int);
bool hasDelivery(int, string);
void addMoney(int, int);
void removePlayerDelivery(int , string, bool);
string getEventName(int i);
void giveChance(int, string);
void removeChance(int, string);
int indexofString(vector<string>, string);
int indexofInt(vector<int>, int);
bool isEmpty(vector<string> );
void primeTime(int);
void changeSpace(int, int);
void addSpaceToPlayer(int, int);
string eventDescription(int);
string getFirstDelivery(int player);

int main()
{
	srand(time(NULL));//make it truely random
	string out;
	for (string s : residences) {
		out += s + " ";
	}
	//cout << out << endl;
	random_shuffle (begin(residences), end(residences));
	out ="";
	for (string s : residences) {
		out += s + " ";
	}
	//cout << out << endl;
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
			cin.ignore(99, '\n');
		}
	}
	vector<string> names(playerCount, "");
	playerNames = names;
	int gettingName = 0;
	while (gettingName < playerCount) {
		cout << "Player " << gettingName + 1 << ", enter your nickname:";
		cin >> names[gettingName];
		if (cin.good()) {
			cout << "Alright! Welcome to the game, " << names[gettingName] << "!" <<endl;
			gettingName++;
		}
		else
		{
			cout << "Invalid input, try again.\n";
			cin.clear();
			cin.ignore(99, '\n');
		}
	}
	//index 0 of the array will be player 1, index one of the array will be player 2
	vector<int> money(playerCount, 200);//set each players' money to 200
	playersMoney = money;//cause you cant split vector declaration and initialization
	
	vector<int> space(playerCount , 1);//set each players' space to 1 (spaces will be 1 indexed)
	//index 1 of the array will be the first space, index 2 will be the second space.
	playersSpace = space;//cause you cant split vector declaration and initialization

	vector<bool> losePackage(playerCount, false);
	porchBandit = losePackage;

	vector<bool> extra(playerCount, false);
	extraPackage = extra;

	vector<int> rolls(playerCount, 0);
	extraRolls = rolls;
	


	
	while (!allDelivered) {
		int timesRolled = extraRolls[playerTurn] + 1;//how many times to rolrolll the dice (plus the one initial roll)

		while (true) {

			std::string response;
			cout << names[playerTurn] << ", what would you like to do?\n";
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
		
		

		//check for event before delivery cause some cards teleport you to houses, so you have to check the delivery on their new space
		
		//non storable events happen now
		if (indexofString(playersEvents[playerTurn], "Traffic") != -1) {
			cout << "You lost your turn cause you got stuck in traffic!" << endl;
			removeChance(playerTurn, "Traffic");
		}
		else//if they dont have the traffic card in their hand
		{
			int diceRoll = 0;//ROLL DICE HERE
			for (int i = 0; i < timesRolled; i++) {
				diceRoll += RNG(1, 6);
			}
			if (timesRolled > 1) {
				extraRolls[playerTurn] = 0;//no extra rolls
			}
			cout << "You rolled a total of " << diceRoll << "!\n";
			addSpaceToPlayer(playerTurn, diceRoll);
		}

		if (indexofString(playersEvents[playerTurn], "Prime Time") != -1) {
			if (!isEmpty(playersDeliveries[playerTurn])) {
				primeTime(playerTurn);
			}
			else
			{
				cout << "You don't have any deliveries!" << endl;
			}
		}
		if (indexofString(playersEvents[playerTurn], "Porch Bandit") != -1) {
			porchBandit[playerTurn] = true;
		}
	
		



		int spaceOn = playersSpace[playerTurn];//space current player is on
		//check for space player is on
		if (isHouse(spaceOn)) {
			cout << "You have landed on the " + getHouseName(spaceOn) << " residence!\n";
			if (hasDelivery(playerTurn, getHouseName(spaceOn))) {//if has a package for residence
				if (!porchBandit[playerTurn]) {
					addMoney(playerTurn, 500);
					cout << "You delivered your package to the " + getHouseName(spaceOn) << " residence and earned $500! \n";
					removePlayerDelivery(playerTurn, getHouseName(spaceOn),false);
				}
				else
				{
					cout << "A porch bandit stole your package!" << endl;
					removeChance(playerTurn, "Porch Bandit");
					removePlayerDelivery(playerTurn, getHouseName(spaceOn), false);

				}
				
			}
		}
		if (isMansion(spaceOn)) {
			cout << "You have landed on the " + getMansionName(spaceOn) << " residence!\n";
			if (hasDelivery(playerTurn, getMansionName(spaceOn))) {
				if (!porchBandit[playerTurn]) {
					addMoney(playerTurn, 1000);
					cout << "You delivered your package to the " + getMansionName(spaceOn) << " residence and earned $1000! \n";
					removePlayerDelivery(playerTurn, getMansionName(spaceOn),false);
				}
				else
				{
					cout << "A porch bandit stole your package!" << endl;
					removeChance(playerTurn, "Porch Bandit");
					removePlayerDelivery(playerTurn, getMansionName(spaceOn), false);

				}
				
			}
		}
		if (isApartment(spaceOn)) {
			cout << "You have landed on the " + getApartmentName(spaceOn) << " residence!\n";
			if (hasDelivery(playerTurn, getApartmentName(spaceOn))) {
				if (!porchBandit[playerTurn]) {
					addMoney(playerTurn, 250);
					cout << "You delivered your package to the " + getApartmentName(spaceOn) << " residence and earned $250! \n";
					removePlayerDelivery(playerTurn, getApartmentName(spaceOn),false);
				}
				else
				{
					cout << "A porch bandit stole your package!" << endl;
					removeChance(playerTurn, "Porch Bandit");
					removePlayerDelivery(playerTurn, getApartmentName(spaceOn), false);

				}
				
			}
		}
		int randomnum = RNG(1,13); //random event
		//check if player is on chance space
		if (find(begin(chanceSpaces),end(chanceSpaces),spaceOn) != end(chanceSpaces)) {
			cout << "You have landed on a chance square! You drew a card: " << getEventName(randomnum) << "\n"; //changed
			giveChance(playerTurn, getEventName(randomnum));
			cout << eventDescription(randomnum);
		}

		//Teleportation check must be between event card and post office check
		if (indexofString(playersEvents[playerTurn], "Teleportation") > -1) {

			bool found = false;
			for (int i = 1; i < 36; i++) {//spaces we will add to the player
				for (int x : postOffices) {
					if (x == spaceOn + i) {
						addSpaceToPlayer(playerTurn, i-1);
						cout << "You have been teleported to the nearest post office!" << endl;
						found = true;
						break;
					}
					
				}
				if (found) {
					break;
				}
			}
			removeChance(playerTurn, "Teleportation");
		}


		//check if player is on post office
		if (find(begin(postOffices), end(postOffices), spaceOn) != end(postOffices)) {
			cout << "You have landed on a post office square!\n";
			givePlayerDelivery(playerTurn, extraPackage[playerTurn]);
			
		}

		


		//TODO: add a check to see what chance cards have to be acted upon the player right now (the non optional ones like losing a package)
		if (indexofString(playersEvents[playerTurn], "Cargo Plane") > -1) {
			extraRolls[playerTurn] = 2;
			removeChance(playerTurn, "Cargo Plane");

		}
		if (indexofString(playersEvents[playerTurn], "Premium Gas") > -1) {
			extraRolls[playerTurn] = 1;
			removeChance(playerTurn, "Premium Gas");

		}
		if (indexofString(playersEvents[playerTurn], "Speeding") > -1) {
			extraRolls[playerTurn] = 1;
			removeChance(playerTurn, "Speeding");

		}
		if (indexofString(playersEvents[playerTurn], "Speeding Ticket") > -1) {
			addSpaceToPlayer(playerTurn, -5);
			removeChance(playerTurn, "Speeding Ticket");
		}
		if (indexofString(playersEvents[playerTurn], "Car Crash") > -1) {
			//cout << playerTurn << endl;;
			string res = getFirstDelivery(playerTurn);
			if (res.compare("") != 0) {
				removePlayerDelivery(playerTurn, res,true);
				cout << "Your delivery to " << res << " was lost due to a car crash!" << endl;
				removeChance(playerTurn, "Car Crash");

				}//has delivery
			else
			{
				cout << "You got off lucky, you had no deliveries to lose." << endl;

			}
			
		}
		if (indexofString(playersEvents[playerTurn], "Pay Raise") > -1) {
			addMoney(playerTurn, 200);
			removeChance(playerTurn, "Pay Raise");

		}
		if (indexofString(playersEvents[playerTurn], "Flat Tire") > -1) {
			addMoney(playerTurn, -200);
			removeChance(playerTurn, "Flat Tire");

		}
		if(indexofString(playersEvents[playerTurn], "Bigger Satchel") > -1) {
			extraPackage[playerTurn] = true;
			removeChance(playerTurn, "Bigger Satchel");

		}
		
		

		//next turn
		cout << endl;
		if (playerTurn + 1 <= playerCount-1) {//get next player's turn
			playerTurn++;
		}
		else
		{
			playerTurn = 0;
		}
		bool allPackagesTaken=true;
		bool allPlayerPackegesDelivered = true;
		for (string s : residences) {

			if (s.compare("TAKEN") != 0) {
				allPackagesTaken = false;
			}
		}

		for (int i = 0; i < playerCount - 1; i++) {
			for (string s : playersDeliveries[i]) {

				if (s.compare("") != 0) {
					allPlayerPackegesDelivered = false;
				}
			}
		}

		if (allPackagesTaken && allPlayerPackegesDelivered) {
			allDelivered = true;
		}
	}

	cout << "The game is now done." << endl;
	int winnerNumber = 0;
	int max = -99999;
	for (int i = 0; i < playerCount; i++) {
		if (playersMoney[i] > max) {
			winnerNumber = i;
			max = playersMoney[i];
		}
			
	}

	cout << "The winner is: " << names[winnerNumber] << endl;
	for (int i = 0; i < playerCount; i++) {
		cout << names[i] << " - $" << playersMoney[i] << endl;
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
	else
	{
		return "NOTVALID";
	}
}

string getMansionName(int i) //Look at 132 to 140
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
	else
	{
		return "NOTVALID";
	}
	
}

string getEventName(int i) {
	if (i == 1) {
		return "Prime Time";
	}
	else if (i == 2) {
		return "Speed Bump";
	}
	else if (i == 3) {
		return "Porch Bandit";
	}
	else if (i == 4) {
		return "Cargo Plane";
	}
	else if (i == 5) {
		return "Speeding";
	}
	else if (i == 6) {
		return "Speeding Ticket";
	}
	else if (i == 7) {
		return "Teleportation";
	}
	else if (i == 8) {
		return "Pay Raise";
	}
	else if (i == 9) {
		return "Car Crash";
	}
	else if (i == 10) {
		return "Traffic";
	}
	else if (i == 11) {
		return "Bigger Satchel";
	}
	else if (i == 12) {
		return "Premium Gas";
	}
	else if (i == 13) {
		return "Flat Tire";
	}
}

string eventDescription(int i) {
	if (i == 1) {
		return "You get a free ride to your delivery space!";
	}
	else if (i == 2) {
		return "You dropped a package!";
	}
	else if (i == 3) {
		return "Your next delivery won't count!";
	}
	else if (i == 4) {
		return "You will get 2 extra rolls on your next turn!";
	}
	else if (i == 5) {
		return "You will roll 1 extra dice on your next turn!";
	}
	else if (i == 6) {
		return "You will be moved 5 spaces back.";
	}
	else if (i == 7) {
		return "You will get teleported to a the nearest post office ahead of you.";
	}
	else if (i == 8) {
		return "You will recive an extra $200.";
	}
	else if (i == 9) {
		return "You have lost a package due to the car crash!";
	}
	else if (i == 10) {
		return "You will lose your next turn.";
	}
	else if (i == 11) {
		return "You will be able to hold one extra package.";
	}
	else if (i == 12) {
		return "You will get an extra roll next round!";
	}
	else if (i == 13) {
		return "You will lose $200.";
	}
}

string getApartmentName(int i)
{
	if (i == 13 || i == 14 || i == 15 || i == 16) {
		return "Lowery";
	}
	else if (i == 31 || i == 32 || i == 33 || i == 34) {
		return "Howells";
	}
	else
	{
		return "NOTVALID";
	}
}

bool givePlayerDelivery(int player, bool goOver) {
	int size = 0;
	int propertiesLeft = 0;
	int retries = 0;
	bool hadToQuit = false;;
	for (string s : residences) {
		size++;
		if (s.compare("") != 0) {
			propertiesLeft++;
		}
	}
	int playerDeliveryAmount = 0;
	for (string s : playersDeliveries[player]) {
		if (s.compare("") != 0) {
			playerDeliveryAmount++;
		}
		
	}
	if ((playerDeliveryAmount <= 5 || goOver) && propertiesLeft > 0) {//has less than 5 deliveries or they draw the card to get an extra delivery
		
		while (residences[deliveryIndex].compare("TAKEN") == 0) {
			if (retries > size) {
				hadToQuit = true;
				break;
			}
			if (deliveryIndex + 1 < size) {
				deliveryIndex++;

			}
			else
			{
				deliveryIndex = 0;
			}
			
			retries++;
			if (residences[deliveryIndex].compare("| E") == 0) {//weird ass error means we have no more deliveries
				hadToQuit = true;
				break;
			}
		}
		if (!hadToQuit) {
			int index = 0;

			for (int i = 0; i < playersDeliveries[player].size(); i++) {//get first index thats not taken

				if (playersDeliveries[player][i] == "") {
					index = i;
					break;
				}
			}
			string indexOfRes = residences[deliveryIndex];
			playersDeliveries[player][index].append(residences[deliveryIndex]);
			cout << "You have been assigned to deliver a package to the " << residences[deliveryIndex] << " residence!" << endl;
			residences[deliveryIndex] = "TAKEN";//set to "TAKEN"
			if (deliveryIndex + 1 < size) {
				deliveryIndex++;

			}
			else
			{
				deliveryIndex = 0;
			}
			if (goOver) {
				extraPackage[playerTurn] = false;
			}
		}
		else
		{
			cout << "There are no more deliveries for you to take!" << endl;
			return false;
		}
		
	}
	else
	{
		cout << "There are no more deliveries for you to take!" << endl;
		return false;

	}
}

int RNG(int low, int high) {
	return rand() % high + low;//return random number
}

inline int fast_rand(void) {//
	g_seed = (214013 * g_seed + 2531011);
	return (g_seed >> 16) & 0x7FFF;
}

bool hasDelivery(int player, string residence) {
	for (int i = 0; i < 6; i++) {
		if (playersDeliveries[player][i].find(residence) != string::npos) {
			return true;
		}
	}
	return false;
}

void addMoney(int player, int amount) {
	if (amount >= 0) {
		playersMoney[player] += amount;

	}
	else
	{
		playersMoney[player] -= amount;

	}
}

void removePlayerDelivery(int player, string residence, bool addToPile) {
	int index = 0;
	for (string s : playersDeliveries[player]) {
		if (s.compare(residence) == 0) {
			playersDeliveries[player][index] = "";
			break;
		}
		index++;
	}

	for (int i = 0; i < size(playersDeliveries[player]); i++) {
		if (playersDeliveries[player][i].compare(residence) == 0) {
			playersDeliveries[player][i] = "";
			break;
		}
	}
	if (playersDeliveries[player][index].compare(residence) == 0) {
		playersDeliveries[player][index] = "";
	}
	if (addToPile) {
		int index = 0;
		for (string s : residences) {
			if (s.compare("TAKEN") == 0) {
				break;
			}
			index++;
		}
		residences[index] = residence;
		
	}
	
}

void giveChance(int player, string card) {

	if (playersEvents[player].size() <= 10) {
		int index = 0;
		for (string s : playersEvents[player]) {
			if (s.compare("") == 0) {
				playersEvents[player][index] = card;
				break;
			}
			index++;
		}

	}
	else
	{
		cout << "You have too many cards! You have been awarded with $200 as compensation.\n";
		addMoney(player, 200);
	}
}

void removeChance(int player, string card) {

	if (playersEvents[player].size() > 0) {
		int index = 0;
		for (string s : playersEvents[player]) {

			if (s.compare(card) == 0) {
				playersEvents[player][index] = "";
				break;
			}
			index++;
		}
	}

}

int indexofString(vector<string> vec, string looking) {
	int index = 0;
	for (string s : vec) {
		if (s.compare(looking) == 0) {
			return index;//break and return index
		}
		index++;
	}
	return -1;//cant find looking
}


int indexofInt(vector<int> vec, int looking) {
	int index = 0;
	for (int s : vec) {
		if (s == looking) {
			return index;//breaks out of loop and returns index
		}
		index++;
	}
	return -1;//if cannot find index of string
}

bool isEmpty(vector<string> vec) {
	for (string s : vec) {
		if (s.compare("") != 0) {
			if (s.compare("TAKEN") != 0) {
				return false;
			}
		}
	}
		return true;
}

void primeTime(int player) {
	int randomDelivery = 0;
	do
	{
		randomDelivery = RNG(0, playersDeliveries[player].size() - 1);

	} while (playersDeliveries[player][randomDelivery].compare("") == 0);//in case we get a "" as our empty value. we cant just use 0 cause the 0th delivery might already be delivered.

	string residence = playersDeliveries[player][randomDelivery];
	int newSpace = 0;
	for (int i = 2; i < 32; i++) {
		if (getApartmentName(i).compare(residence) == 0) {
			newSpace = i;
			break;
		}
		if (getHouseName(i).compare(residence) == 0) {
			newSpace = i;
			break;
		}
		if (getMansionName(i).compare(residence) == 0) {
			newSpace = i;
			break;
		}
	}

	playersSpace[player] = newSpace;

	removeChance(player, "Prime Time");
}

void changeSpace(int player, int space) {
	playersSpace[player] = space;
}


void addSpaceToPlayer(int player, int space) {	
	if (space >= 0) {
		playersSpace[player] += space;
		if (playersSpace[player] > 36) {
			playersSpace[player] = playersSpace[player] - 36;//set you to the start of the board. start space is 1

		}
	}
	else
	{
		playersSpace[player] += space;
		if (playersSpace[player] < 0) {
			playersSpace[player] = playersSpace[player] + 36;//put you at the back of the board

		}
	}
	
}
string getFirstDelivery(int player) {
	string removalDelivery = "";
	for (int i = 1; i <= 36; i++) {
		if (hasDelivery(player, getApartmentName(i))) {
			return getApartmentName(i);
		}
		else if (hasDelivery(player, getMansionName(i))) {
			removalDelivery = getMansionName(i);
			return getMansionName(i);
		}
		else if (hasDelivery(player, getHouseName(i))) {
			removalDelivery = getHouseName(i);
			return getHouseName(i);
		}
	}
	return "";
	}


