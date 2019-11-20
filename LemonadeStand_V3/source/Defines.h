/* Defins.h
 * Programmer: Paul King
 * Date Completed: 4/11/2010
 * Description: This file contains all the headers used throughout the program along with it's
 *				constants and enumerations and structures.
 *				All the global objects, variables and arrays are declared in Defines.cpp
*/

/****************
 * HEADER FILES *
 ****************/
#include <iostream> // needed for most operations
	using namespace::std; // allows typing of cout, cin & endl without std:: each time

#include <iomanip> // needed to use setprecision
#include <ctime> // need to use time function
#include <conio.h> // needed to use the _kbhit and _getch functions
#include <windows.h> // needed to get the system's date and time

#pragma once // this header can only be included once by each file that uses it

/**************
 * CONSTANTS  *
 **************/
// individual units of an item per another item
const unsigned short LEMONS_PER_KILO = 8, // 8 lemons per kilogram
					 SUGAR_PER_BAG = 8, // 8 cups or sugar for every bag					 
					 ICE_CUPS_PER_BAG = 10, // 10 cups of ice for every bag
					 ICE_CUBES_PER_CUP = 6, // 6 cubes of ice for every cup and 60 cubes of ice for every bag
					 CUPS_PER_JUG = 6; /* 6 cup of lemonade for every jug (must be float for some reason
										  in order to divde properly in this program) */
			
// minimum and maximum prices (mesured in cents) when buying stock from the store
const unsigned short PRICE_MIN_LEMON = 240, // cost per kilo of lemons
					 PRICE_MAX_LEMON = 480,
					 PRICE_MIN_SUGAR = 160, // cost per bag of sugar (8 cups)
					 PRICE_MAX_SUGAR = 320,
					 PRICE_MIN_ICE = 110, // cost per bag of ice (10 cups)
					 PRICE_MAX_ICE = 220;

// the max and min number of recipe items
const float RECIPE_MAX_LEMONS = 10.0f,
			RECIPE_MIN_LEMONS = 1.0f,
			RECIPE_MAX_SUGAR = 10.0f,
			RECIPE_MIN_SUGAR = 0.25f;
const unsigned short RECIPE_MAX_ICE = 10,			
					 RECIPE_MIN_ICE = 0;

// the max and min lemonade price
const float PRICE_LEMONADE_MAX = 10.0f,
			PRICE_LEMONADE_MIN = 0.10f;	

// the max and min of items that can be bought or destroyed and the weather temperature in celcius
const unsigned short TEMPERATURE_MAX = 45,
					 TEMPERATURE_MIN = 10;

// the default recipe and sale price values
const unsigned short DEFAULT_RECIPE_LEMONS = 3,
					 DEFAULT_RECIPE_SUGAR = 2,
					 DEFAULT_RECIPE_ICE = 3;
const float DEFAULT_LEMONADE_PRICE = 1.10f;

// the amount of money the player starts with
const float STARTING_MONEY_EASY = 200,
			STARTING_MONEY_MEDIUM = 100,
			STARTING_MONEY_HARD = 50;

// the length of the first day based on the difficulty level
const unsigned int STARTING_LENGTH_EASY = 210,
					 STARTING_LENGTH_MEDIUM = 120,
					 STARTING_LENGTH_HARD = 60;

// the amount of money the player needs to win the game
const float TARGET_MONEY_SHORT = 500,
			TARGET_MONEY_MEDIUM = 1000,
			TARGET_MONEY_LONG = 5000;

// time lengths in days that each stock lasts before going off or melting
const unsigned short TIME_LENGTH_LEMONS = 3,
					 TIME_LENGTH_SUGAR = 14,
					 TIME_LENGTH_ICE = 1,
					 TIME_LENGTH_LEMONADE = 1;

// the maximum amount that a customer with 0 cheapness will pay for a cup of lemonade
const float BASE_MAX_PRICE = 1.00;

// array index sizes
const unsigned short MAX_STOCK_PURCHASES = 50; // the max number of individual stock purchases
					 
/****************
 * ENUMERATORS  *
 ****************/
enum GAME_STATE // the state the game is currently in
{
	Initialising,
	Running,
	Ending

}; // end of game state enumerator

enum MAIN_MENU
{
	Exit,
	New_Game,
	Load_Game,
	Instructions,
	About	

}; // end of main menu enumerator

enum GAME_MENU // the decision the player makes on the main menu
{
	End_Game,
	Open_Store,
	See_Finances,
	View_Stock,
	Set_Recipe_Or_Price,
	Make_Lemonade,
	Save_Game,
	Debug_Menu

};// end of game menu enumerator

enum ITEM_SELECTED
{
	Lemons,
	Sugar,
	Ice,
	Lemonade,
	Exit_Screen
}; // end of item selected enumerator

enum STORE_STATUS
{
	Buy_Stock,
	Store_Open,
	Out_Of_Stock
}; // end of store status enumerator

enum WEATHER_OVERCAST
{
	No_Clouds,
	Some_Clouds,
	Full_Overcast
}; // end of weather overcast enumerator

enum WEATHER_TEMPERATURE
{
	Cold,
	Warm,
	Hot,
	Very_Hot
}; // end of weather temperature enumerator

// the sweet and sour level of the recipe
enum RECIPE_SWEETNESS
{
	Very_Sour,
	Sour,
	Slightly_Sour,
	Average,
	Slightly_Sweet,
	Sweet,
	Very_Sweet
};

// the warmth of the lemonade depending on the amount of ice and daily temperature
enum RECIPE_TEMPERATURE
{
	Very_Cold,
	Somewhat_Cold,
	Slightly_Cold,
	Ideal,
	Slightly_Warm,
	Somewhat_Warm,
	Very_Warm	
};

/*****************
*   Structures   *
******************/
struct StockItems
{
	float lemons, sugarBags, sugarCups, iceBags, iceCups, lemonadeJugs;
	short iceCubes, lemonadeCups;
};

struct Recipe
{
	float lemon, sugar; 
	short ice;
};

struct Pricing
{
	float lemons, sugarBags, iceBags, lemonadePrice, lemonsPerCup, sugarPerCup, icePerCup, icePerJug, totalPerCup;
};

struct Finances
{
	unsigned short customers, cupsSold, cupsLost, cupsDestroyed, priceGood, priceTooHigh, recipeGood,
				   recipeBad, iceMelted, sugarInfestAnts;
	float moneyMade, moneySpent, totalProfit, jugsSold, jugsLeftover, lemonsOff, sugarOff, lemonsInfested, 
		  sugarInfestRodents;
};

struct StockList
{
	ITEM_SELECTED itemType;
	char* itemName;
	float quantity;
	unsigned short dayBought, daysIn, daysLeft;
};

struct Weather_Forecast
{
	unsigned short tempCel;
	unsigned short tempFah;
	WEATHER_TEMPERATURE tempRange;
	WEATHER_OVERCAST overcast;
	bool rain;
};

struct Customers
{
	unsigned short delay, // the number of seconds between each customer arriving
				   cheapness, // how fussy the current customer is with the price
				   recipePreference, // how fussy the current customer is with the recipe
				   iceTolerance;
};

struct Date
{
	unsigned int day, month, year;
	char dateString[11];
};

struct PlayerData
{
	//bool saveEnabled;	
	char playerName[17];
	unsigned short dayNum;
	unsigned int dayLength;
	unsigned int dayLengthIncrease;
	float startingMoney;
	float targetMoney;
	float currentMoney;
	StockItems stock;
	Recipe recipe;
	Pricing prices;
	Finances finance;
	Weather_Forecast weather;
	Date date;

	//StockList stockDays[MAX_STOCK_PURCHASES];
	unsigned short stockPurchases;
};

struct HighScores
{
	unsigned short rank;
	char name[17];
	float totalMoney;
	unsigned short daysUsed;
	char* date;
};

/***********************
 * Function prototypes *
 ***********************/
void InitialiseArrays();
void InitialisePlayerData();
void PauseAndClear();
bool CheckEntryFormat(char cReceived[]);
void underDevelopment();
short GetNextStockVacancy();
void AddToStock(ITEM_SELECTED eItemUsed, float fItemAmount);
void RemoveFromStock(ITEM_SELECTED eItemUsed, float fItemAmount);