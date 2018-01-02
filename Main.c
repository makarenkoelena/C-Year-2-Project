//libraries
#include <stdio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

//constants:
#define MIN_NUM_OF_PLAYERS  2
#define MAX_NUM_OF_PLAYERS  10
#define NUM_OF_CARDS  13
#define NUM_OF_ROUNDS  2
#define NAME_LENGTH  10

// structs
typedef struct Player {
	int  id;
	int score;
	char name[NAME_LENGTH];
} Player;

typedef struct Card {
	int value;
	int suit;
} Card;

//functions
//void instructions();
int getNumOfPlayers();
void createPlayers(int numOfPlayers, Player players[MAX_NUM_OF_PLAYERS]);
void displayPlayers(int numOfPlayers, Player players[MAX_NUM_OF_PLAYERS]);
void diplayPlayersCards(Player players[MAX_NUM_OF_PLAYERS], Card playersCardsData[MAX_NUM_OF_PLAYERS][NUM_OF_CARDS], int numOfPlayers);
void diplaySinglePlayerCards(Player players[MAX_NUM_OF_PLAYERS], Card playersCardsData[MAX_NUM_OF_PLAYERS][NUM_OF_CARDS], int playerIndex);
Card pickCard(Card playersCardsData[MAX_NUM_OF_PLAYERS][NUM_OF_CARDS], int numOfPlayers);
void dealCards(Card playersCardsData[MAX_NUM_OF_PLAYERS][NUM_OF_CARDS], int numOfPlayers);
void displayChosenCards(Player players[MAX_NUM_OF_PLAYERS], Card cardsOnTheTable[NUM_OF_CARDS]);
//bool findSameCards(Card cardsOnTheTable[NUM_OF_CARDS], int numOfPlayers);
int findTheRoundWinner(Card cardsOnTheTable[NUM_OF_CARDS], int numOfPlayers);
int calcRoundTotal(Card cardsOnTheTable[NUM_OF_CARDS], int numOfPlayers);
int calcGameTotal(Player players[MAX_NUM_OF_PLAYERS], int numOfPlayers);
int findTheGameWinner(Player players[MAX_NUM_OF_PLAYERS], int numOfPlayers);
void clearScreen();
void optionToSaveTheGame(char fileName[25], int numOfRounds, int numOfPlayers, Card playersCardsData[MAX_NUM_OF_PLAYERS][NUM_OF_CARDS], Player players[MAX_NUM_OF_PLAYERS]);
void saveGame(char fileName[25], int numOfRounds, int numOfPlayers, Card playersCardsData[MAX_NUM_OF_PLAYERS][NUM_OF_CARDS], Player players[MAX_NUM_OF_PLAYERS]);
//=========================================================== Here Main Starts ==================================================================================================
void main()
{
	// variables here:
	Player players[MAX_NUM_OF_PLAYERS];
	Card playersCardsData[MAX_NUM_OF_PLAYERS][NUM_OF_CARDS];
	Card cardsOnTheTable[NUM_OF_CARDS];
	Card playersScoreData[MAX_NUM_OF_PLAYERS][NUM_OF_CARDS];
	srand(time(NULL));//seed required for "randomness"
	int again = 1;
	int numOfPlayers;
	int id;
	int roundCounter = 0;
	//=====================
	do {
		numOfPlayers = getNumOfPlayers();
		createPlayers(numOfPlayers, players);
		//displayPlayers(numOfPlayers, players);
		dealCards(playersCardsData, numOfPlayers);
		diplayPlayersCards(players, playersCardsData, numOfPlayers);

		for (int round = 0; round < NUM_OF_ROUNDS; round++) {
			roundCounter++;
			printf("\n");
			printf("Round %d: \n", round + 1);
			printf("=======================================\n");

			// each player choses the card (index) and puts the card on the table
			for (int playerIndex = 0; playerIndex < numOfPlayers; playerIndex++) {
				printf("Player %d: \n", playerIndex + 1);
				diplaySinglePlayerCards(players, playersCardsData, playerIndex);




				//this part doesn't work
				do {
					printf("Please enter card's id: \n");
					scanf("%d", &id);
				} while (id < 0 || id > NUM_OF_CARDS || playersCardsData[playerIndex][id].value != NULL);// || playersCardsData[playerIndex][i].value = NULL <-- the user cant choose the card which he has already picked in previous rounds (they are assigned to null/0
				//tried to put this stuff into a separate method, failed
				cardsOnTheTable[playerIndex] = pickCard(playersCardsData, numOfPlayers);

				//populate Card cardsOnTheTable[NUM_OF_CARDS] with chosen cards
				// cardsOnTheTable[playerIndex].value = playersCardsData[playerIndex][id - 1].value;
				//printf("card's value is: %d \n", cardsOnTheTable[playerIndex].value);
				//clearScreen();

				// delete the chosen cards (actually, assign them to zero) and show the rest of the cards
				for (int i = 0; i < NUM_OF_CARDS; i++) {
					if (i == (id - 1)) {
						playersCardsData[playerIndex][i].value = NULL;
					}
					//printf(" %d ", playersCardsData[playerIndex][i].value);
				}
				printf("\n");
			}//for players

			displayChosenCards(players, cardsOnTheTable);
			//findSameCards(cardsOnTheTable, numOfPlayers);
			printf("\n");

			// find the round winner and assign the score
			for (int i = 0; i < numOfPlayers; i++) {
				if (i == findTheRoundWinner(cardsOnTheTable, numOfPlayers)) {
					players[i].score += calcRoundTotal(cardsOnTheTable, numOfPlayers);
				}
			}
			int roundWinner = findTheRoundWinner(cardsOnTheTable, numOfPlayers);
			printf("Winner of this round is %d\n", roundWinner + 1);
			printf("Score of this round is: %d\n", calcRoundTotal(cardsOnTheTable, numOfPlayers));

			displayPlayers(numOfPlayers, players);
		} //for rounds
		int gameWinner = findTheGameWinner(players, numOfPlayers);
		printf("The winner is the %d player with the score %d: \n", gameWinner + 1, calcGameTotal(players, numOfPlayers));
		//=============================================================================================
		printf("Enter 1 to continue or 0 to exit:\n");
		scanf("%d", &again);

		if (again == 0) {
			//int fileName, numOfRounds;//how to call optionToSaveTheGame method?
			//optionToSaveTheGame(fileName, numOfRounds, numOfPlayers, playersCardsData, players);
		}
	} while (again == 1);
	getchar();
}// end of main
 //======================================================== end of main ==============================================================================================================
int getNumOfPlayers() {
	int numOfPlayers;
	//1. User enters number of players in the range of 2-10
	printf("Please enter the number of players: \n");
	scanf("%d", &numOfPlayers);
	//printf("Test: number of players is: %d \n", numOfPlayers);
	while (numOfPlayers < MIN_NUM_OF_PLAYERS || numOfPlayers > MAX_NUM_OF_PLAYERS) {
		printf("Wrong value. Please enter the number between 2 and 10: \n");
		scanf("%d", &numOfPlayers);
		//printf("Test: number of players is: %d \n", numOfPlayers);
	}
	return numOfPlayers;
}
//=====================================================================================
void createPlayers(int numOfPlayers, Player players[MAX_NUM_OF_PLAYERS]) {
	for (int i = 0; i < numOfPlayers; i++) {
		printf("Enter name for the %d player: ", i + 1);
		scanf("%s", players[i].name);
		players[i].score = 0;
		players[i].id = i;
	}
}
//=================================================
void displayPlayers(int numOfPlayers, Player players[MAX_NUM_OF_PLAYERS]) {
	for (int i = 0; i < numOfPlayers; i++) {
		printf("%s your score is %d \n", players[i].name, players[i].score);
		//printf("%d\n", players[i].id);
	}
}
//==================================================================
void diplayPlayersCards(Player players[MAX_NUM_OF_PLAYERS], Card playersCardsData[MAX_NUM_OF_PLAYERS][NUM_OF_CARDS], int numOfPlayers) {
	printf("PLAYERS DATA \n");
	printf("%3s %10s |", " ", " ");
	for (int i = 0; i < NUM_OF_CARDS; i++) {
		printf("%2d   |", i + 1);
	}
	printf("\n----------------------------------------------------------------------------------------------\n");

	for (int playerIndex = 0; playerIndex < numOfPlayers; playerIndex++) {
		printf("%3s %10s |", " ", players[playerIndex].name);

		for (int card = 0; card < NUM_OF_CARDS; card++) {
			switch (playersCardsData[playerIndex][card].suit) {
			case 0:
				printf("%2c", 'H');
				break;
			case 1:
				printf("%2c", 'D');
				break;
			case 2:
				printf("%2c", 'S');
				break;
			case 3:
				printf("%2c", 'C');
				break;
			}
			if (1 < playersCardsData[playerIndex][card].value && playersCardsData[playerIndex][card].value < 11) {
				printf("%2d |", playersCardsData[playerIndex][card].value);
			}
			else {
				switch (playersCardsData[playerIndex][card].value) {
				case 11:
					printf("%2c |", 'J');//jack
					break;
				case 12:
					printf("%2c |", 'Q');//queen
					break;
				case 13:
					printf("%2c |", 'K');//king
					break;
				case 14:
					printf("%2c |", 'A');//ace
					break;
				}
			}
		}// cards
		printf("\n");
	}
	printf("\n");
}
//======================================================
void diplaySinglePlayerCards(Player players[MAX_NUM_OF_PLAYERS], Card playersCardsData[MAX_NUM_OF_PLAYERS][NUM_OF_CARDS], int playerIndex) {

	printf("%3s %10s |", " ", "ID:");
	for (int cardIndex = 1; cardIndex <= NUM_OF_CARDS; cardIndex++) {
		printf(" %2d  |", cardIndex);
	}
	printf("\n----------------------------------------------------------------------------------------------\n");

	printf("\n");
	printf("%3s %10s |", " ", players[playerIndex].name);

	for (int card = 0; card < NUM_OF_CARDS; card++) {
		switch (playersCardsData[playerIndex][card].suit) {
		case 0:
			printf("%2c", 'H');
			break;
		case 1:
			printf("%2c", 'D');
			break;
		case 2:
			printf("%2c", 'S');
			break;
		case 3:
			printf("%2c", 'C');
			break;
		}
		if (1 < playersCardsData[playerIndex][card].value && playersCardsData[playerIndex][card].value < 11 || playersCardsData[playerIndex][card].value == NULL) {//remove chosen cards
			printf("%2d |", playersCardsData[playerIndex][card].value);
		}
		else {
			switch (playersCardsData[playerIndex][card].value) {
			case 11:
				printf("%2c |", 'J');
				break;
			case 12:
				printf("%2c |", 'Q');
				break;
			case 13:
				printf("%2c |", 'K');
				break;
			case 14:
				printf("%2c |", 'A');
				break;
			}
		}
	}// cards
	printf("\n");
}
//=========================================================
void dealCards(Card playersCardsData[MAX_NUM_OF_PLAYERS][NUM_OF_CARDS], int numOfPlayers) {
	int card;
	int randSuit, randValue;

	// deal each player a card of each suit first:
	for (int player = 0; player < numOfPlayers; player++) {
		for (card = 0; card < 4; card++) {
			playersCardsData[player][card].suit = card;//number from 0 till 3 --> at least  1 of each suit
			//printf("%2d | ", playersCardsData[player][card].suit);
			playersCardsData[player][card].value = rand() % 12 + 2;//generates random numbers from 2 till 14
			//printf("%2d |", playersCardsData[player][card].value);
		}//inner for
		//deal the rest of the cards
		for (card = 4; card < 14; card++) {
			do {
				randSuit = rand() % 4;//generates random numbers from 0 till 3 --> suits
				playersCardsData[player][card].suit = randSuit;
				//printf("%2d | ", playersCardsData[player][card].suit);
				randValue = rand() % 12 + 2;//generates random numbers from 2 till 14 --> values
				playersCardsData[player][card].value = randValue;
				//printf("%2d |", playersCardsData[player][card].value);
			} while (playersCardsData[player][card].suit != randSuit && playersCardsData[player][card].value != randValue);
		}//inner for
		printf("\n");
	}//outer for
}
//==============================================================
Card pickCard(Card playersCardsData[MAX_NUM_OF_PLAYERS][NUM_OF_CARDS], int numOfPlayers) {
	Card pickedCard;
	int id;
	for (int playerIndex = 0; playerIndex < numOfPlayers; playerIndex++) {
		// clearScreen();
		do {
			printf("Please enter card's id: \n");
			scanf("%d", &id);
		} while (id < 0 || playersCardsData[playerIndex][id].value == NULL || playerIndex < id);

		pickedCard = playersCardsData[playerIndex][id];
		printf("card's id is: %d \n", id);
		// TODO: add some hold functionality. Like "Enter 'c' to continue"
	}
	return pickedCard;
}
//==============================================================
void displayChosenCards(Player players[MAX_NUM_OF_PLAYERS], Card cardsOnTheTable[NUM_OF_CARDS])
{
	printf("%10s|", "Cards to compare: ");
	for (int card = 0; card < NUM_OF_CARDS; card++) {
		if (1 < cardsOnTheTable[card].value && cardsOnTheTable[card].value < 11) {
			printf("%d %c|", cardsOnTheTable[card].value, cardsOnTheTable[card].suit);
		}
		else {
			switch (cardsOnTheTable[card].value) {
			case 11:
				printf("%2c |", 'J');
				break;
			case 12:
				printf("%2c |", 'Q');
				break;
			case 13:
				printf("%2c |", 'K');
				break;
			case 14:
				printf("%2c |", 'A');
				break;
			}
		}
	}
	printf("\n");
}
//================================================================================
//bool findSameCards(Card cardsOnTheTable[NUM_OF_CARDS], int numOfPlayers)
//{
//	for (int cardIndex = 0; cardIndex < numOfPlayers; cardIndex++) {
//		for (int j = 0; j < cardIndex; j++)
//		{
//			if (cardsOnTheTable[cardIndex].value == cardsOnTheTable[j].value && cardIndex != j)
//			{
//				//printf("Same");
//				return true;
//			}
//			else {
//				//printf("Different");
//				return false;
//			}
//		}
//	}
//}
//=============================================================================================
//code taken from: http://www.cyberforum.ru/c-beginners/thread1740100.html
int findTheRoundWinner(Card cardsOnTheTable[NUM_OF_CARDS], int numOfPlayers)
{
	//THIS PART DOESNT WORK PROPERLY AS WELL,  IF THE CARDS ARE EQUAL, THE WINNER WILL BE THE FIRST (OUT OF ALL PLAYERS WHO HAVE THE SAME CARDS)PLAYER IN THE ROW
	//bool thereIsaWinner;???
	int cardsToCompare[13], i, j, *p, flag = 0;
	int largestCard = 1, winnerPos = 0;
	for (i = 0; i < numOfPlayers; i++) {
		cardsToCompare[i] = cardsOnTheTable[i].value;
		//printf("%3d", cardsToCompare[i]);
	}
	p = &cardsToCompare[0];
	for (int i = 0; i < numOfPlayers - 1; i++) {
		for (j = i + 1; j < numOfPlayers; j++) {
			if (*(p + i) == *(p + j)) {
				*(p + j) = 0;
				flag = 1;
			}
		}
		if (flag) {
			*(p + i) = 0;
			flag = 0;
		}
	}
	printf("\n");
	for (i = 0; i < numOfPlayers; i++) {
		//printf("%3d", cardsToCompare[i]);
	}

	for (int i = 0; i < numOfPlayers; i++) {
		if (cardsToCompare[i] > largestCard) {
			largestCard = cardsToCompare[i];
			winnerPos = i;
		}
	}
	return winnerPos;
}
//================================================
int calcRoundTotal(Card cardsOnTheTable[NUM_OF_CARDS], int numOfPlayers) {
	int roundTotal = 0;
	for (int cardIndex = 0; cardIndex < numOfPlayers; cardIndex++) {
		roundTotal += cardsOnTheTable[cardIndex].value;
	}
	return roundTotal;
}
//=====================================================================================
int calcGameTotal(Player players[MAX_NUM_OF_PLAYERS], int numOfPlayers)
{
	int highestScore;
	for (int i = 0; i < numOfPlayers; i++) {
		highestScore = players[0].score;
		if (players[i].score > highestScore) {
			highestScore = players[i].score;
		}//if
	}//for
	return highestScore;
}
//=====================================================
int findTheGameWinner(Player players[MAX_NUM_OF_PLAYERS], int numOfPlayers)
{

	int highestScore;
	int gameWinnerPos = 0;
	for (int i = 0; i < numOfPlayers; i++) {
		highestScore = players[0].score;
		if (players[i].score > highestScore) {
			highestScore = players[i].score;
			gameWinnerPos = i;
		}//if
	}//for
	return gameWinnerPos;
}//findTheGameWinner()
 //=====================================================
void clearScreen()
{
	for (int i = 0; i < 50; i++)
	{
		printf("\n");
	}
}
//==============================================
void optionToSaveTheGame(char fileName[25], int numOfRounds, int numOfPlayers, Card playersCardsData[MAX_NUM_OF_PLAYERS][NUM_OF_CARDS], Player players[MAX_NUM_OF_PLAYERS]) {
	char answer;
	// char fileName[15];
	printf("Do you want to save this game? (Y (yes) / N (no) \n");
	scanf(" %c", &answer);
	//printf("Answer is: %c \n", answer);

	switch (answer) {
	case 'y':
	case 'Y':
		printf("Please enter file's name: \n");
		scanf("%s", fileName);
		//printf("Name is: %s \n", fileName);
		saveGame(fileName, numOfRounds, numOfPlayers, playersCardsData, players);
		break;
	case 'n':
	case 'N':
		exit(0);//"avtovihod"
		break;
	}
}
//==============================================
void saveGame(char fileName[25], int numOfRounds, int numOfPlayers, Card playersCardsData[MAX_NUM_OF_PLAYERS][NUM_OF_CARDS], Player players[MAX_NUM_OF_PLAYERS]) {
	FILE* pFile;
	pFile = fopen(fileName, "w");

	if (pFile == NULL) {
		printf("The file cannot be opened\n");
	}
	else
	{
		// write everything to file:
		fprintf(pFile, "%d\n", numOfRounds);
		fprintf(pFile, "%d\n", numOfPlayers);

		//info about players
		for (int playerIndex = 0; playerIndex < numOfPlayers; playerIndex++) {
			fprintf(pFile, "%d %s %d", players[playerIndex].id, players[playerIndex].name, players[playerIndex].score);
			//info about cards
			for (int cardIndex = 0; cardIndex < NUM_OF_CARDS; cardIndex++) {
				fprintf(pFile, " %c %d", playersCardsData[playerIndex][cardIndex].suit, playersCardsData[playerIndex][cardIndex].value);
			}//inner for
			fprintf(pFile, "\n");
		}//outer for
		fclose(pFile);
	}//else
}//saveGame
//==============================================
//void uploadGame()
//{
//
//}
