/* Customer.h
 * Programmer: Paul King
 * Date Completed: 4/11/2010
 * Description: This is the header file for Customer.cpp which holds all the header files,
 *				external variables and objectsand constants that the source file uses.
 *				The protoypes for all the functions called within the file is also located here.		
*/

// include headers
#include "defines.h"

#pragma once // this header can only be included once by each file that uses it

/***************
 * Enumerators *
 ***************/
// the sweet and sour level that the customer wants
enum TASTE_PREFERENCE
{
	Sour_Only,
	Sour_And_Standard,
	Standard,
	Sweet_And_Standard,
	Sweet_Only
};

// values for how the cutomer found the recipe
enum TASTE_RESULT
{
	Good,
	Too_Sour,
	Too_Sweet,
	Too_Cold,
	Too_Warm
};

enum STORE_EVENT
{
	Knocked_Over,	
	Rodent,
	Ball,
	Steal,
	Homeless
};

/*************
 * Constants *
 *************/
// the chances of an items being destroyed while the store is open
const unsigned short //PERCENT_CHANCE_RODENT = 5, // chance of a rodent infesting the sugar
					 PERCENT_CHANCE_ANTS = 20, // chance of ants infesting the sugar
					 PERCENT_CHANCE_KID = 20, // chance of kids knocking over lemonade
					 PERCENT_CHANCE_EVENT = 50; 

/* the minimum and maximum cups that can be bought and can be destroyed and
   the minimum and maximum lemons and sugar that can be infested by rodents */
const unsigned short CUPS_BOUGHT_MIN = 1,
					 CUPS_BOUGHT_MAX = 5,
					 DESTROYED_CUPS_MIN = 1,
					 DESTROYED_CUPS_MAX = 6,
					 DESTROYED_LEMONS_MIN = 1,
					 DESTROYED_LEMONS_MAX = 6,
					 DESTROYED_SUGAR_MIN = 1,
					 DESTROYED_SUGAR_MAX = 8;

const unsigned short KID_KNOCKED_MIN = 1,
					 KID_KNOCKED_MAX = 6,					 
					 BALL_KNOCKED_MIN = 1,
					 BALL_KNOCKED_MAX = 6,
					 STOLEN_CUPS_MIN = 1,
					 STOLEN_CUPS_MAX = 2,
					 HOMELESS_GIVEN_MIN = 1,
					 HOMELESS_GIVEN_MAX = 5;

// the number of lines allowed in the customer sales message before	scrolling
const unsigned short MAX_LINES = 12;

/****************************************
 * Global varibles and character arrays *
 ****************************************/
// the current message being sent to to the full message array
char strCurrentMessage[100];
// string array to store the sales message
char strStoreInfo[15][100];
// global variable for amount of lines that have been used in the store info string
unsigned short sLinesUsed = 0;

/**********************************
 * External variables and objects *
 **********************************/
// get external structure objects created in defines.cpp
extern PlayerData data;
extern StockItems stock;
extern Recipe recipe;
extern Pricing prices;
extern Finances financeTotal;
extern Finances financeDaily;
extern Weather_Forecast weather;
extern StockList stockDays[];
extern Customers customer;

extern bool DebugMode;
// get external enumerator variables
extern STORE_STATUS eStoreStatus;
extern char Input;

/***********************
 * Function prototypes *
 ***********************/
void AddStoreMessage(char* cMessage);
void UpdateStockTimers();
void DestroyStock(float stockAvailable, ITEM_SELECTED itemUsed, char* causer);
unsigned short ProcessNewCustomer();
void ProcessCupsBought(unsigned int sPriceSatisfaction);
unsigned short GetPriceDecision();
TASTE_RESULT GetTasteApproval();
RECIPE_SWEETNESS GetRecipeSweetness();
RECIPE_TEMPERATURE GetRecipeTemperature();
void ShowSalesInfo(unsigned short sTicksPassed, unsigned short sTicksLeft);
void GetEvent();
void EndDay(unsigned short ticksLeft, unsigned short timeLength);
void ShowDailyReport(unsigned short ticksLeft, unsigned short timeLength);
void CheckLinesUsed();
void GetIncidentChance();
void ShowStockList();
void ShowWeather();