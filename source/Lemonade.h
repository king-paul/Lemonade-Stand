/* Lemonade.h
 * Programmer: Paul King
 * Date Complete: 4/11/2010
 * Description: This is the header file for Lemonade.cpp which holds all the header files and
 *				external variables and object that source file uses. The protoypes for all the
 *				functions called within the file is also located here.
*/

// include built in headers used in Lemonade.cpp
#include <iostream>
#include <cmath>
#include "defines.h"

#pragma once // this header can only be included once by each file that uses it

using namespace::std;

/*************
 * CONSTANTS *
 *************/
const short PERCENT_LEMONS_MAX = 15,
			PERCENT_SUGAR_MAX = 10,
			PERCENT_ICE_MAX = 50;

/**********************************
 * EXTERNAL VARIABLES AND OBJECTS *
 **********************************/
// get external variables
extern STORE_STATUS eStoreStatus;
extern bool DebugMode;

// get external structure objects created in defines.cpp
extern PlayerData data;
extern StockItems stock;
extern Recipe recipe;
extern Pricing prices;
extern Finances financeTotal;
extern StockList stockDays[];
extern Finances financeDaily;

// global variables
bool blValidation; // variable that checks the validation of numerical input
extern char cInput; // used for user character input

/***********************
 * Function prototypes *
 ***********************/
void DisplayStockItems();
void DisplayStockAndPrices();
float UpdateItemValue(ITEM_SELECTED itemChosen, char stringFound[], float defaultValue);
void SelectStockAutomatically();
void SelectStockManually();
void ProcessPurchase(ITEM_SELECTED itemChosen, float quantity, float totalPrice);
void MakeLemonade();
unsigned short CalculateMaximumJugs();
void ProcessLemonadeJugs(unsigned short sJugsToMake);
float GetRandomStockLoss(ITEM_SELECTED itemType);
void ShowStockList(); // currenlty unused
// functions from Customer.cpp
RECIPE_SWEETNESS GetRecipeSweetness();
RECIPE_TEMPERATURE GetRecipeTemperature();