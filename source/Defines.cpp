/* Defines.cpp
 * Programmer: Paul King
 * Last Updated: 4/11/2010
 * Description: This file contains all the global structures that are used in the program.
 *				General functions called from more then one file are also be found here.
 *				Defines.h contains all the constants and enumerations structured for the program.
*/

// include headers
#include "defines.h"

/****************************************
 * Global variables used in other files *
 ****************************************/
bool DebugMode = false; // determins whether the debug information will be shown
char cInput; // varible for character input to be used by all files
STORE_STATUS eStoreStatus; // enumerator for the status of the store

/******************************
*  Declare Structure Objects  *
*******************************/
PlayerData data;
PlayerData savedData[10];
StockItems stock;
Recipe recipe;
Pricing prices;
Finances financeDaily;
Finances financeTotal;
Weather_Forecast weather;
StockList stockDays[MAX_STOCK_PURCHASES];
Customers customer;
SYSTEMTIME systemTime;

/*******************************
 * functions used by all files *
 *******************************/
// function that sets all the game variable starting values
void InitialisePlayerData()
{
	// set stock defaults
	stock.lemons = 0;
	stock.sugarBags = 0;
	stock.sugarCups = 0;
	stock.iceBags = 0;
	stock.iceCubes = 0;
	stock.lemonadeJugs = 0;
	stock.lemonadeCups = (unsigned short)(stock.lemonadeJugs * CUPS_PER_JUG);
	
	// set recipe defaults
	recipe.lemon = DEFAULT_RECIPE_LEMONS;
	recipe.sugar = DEFAULT_RECIPE_SUGAR;
	recipe.ice = DEFAULT_RECIPE_ICE;

	// set dafault sale price for lemonade
	prices.lemonadePrice = DEFAULT_LEMONADE_PRICE;
	financeTotal.cupsSold = 0;
	financeTotal.moneyMade = 0;
	financeTotal.moneySpent = 0;
	financeTotal.totalProfit = 0;

	// set other data values
	data.dayNum = 1;
	//data.saveEnabled = true;

} // end of InitialisePlayerData function

// function that runs the pause and clear system functions
void PauseAndClear()
{
	system("pause"); // pause the system and wait for keypress
	system("cls"); // clear the screen
} // end of PauseAndClear function

// function that validates entry to ensure that it is numeric
bool CheckEntryFormat(char cReceived[])
{
	float fParsedValue;

	try // check for exceptions
	{
		for(int i=0; i<(int)strlen(cReceived); i++) // check all characters in the string
		{
			if((isdigit(cReceived[i])) || (cReceived[i] == '.') || (cReceived[i] == '-'))
			{ 
				fParsedValue = (float) atof(cReceived);				
			}
			else
			{
				throw 0; // thorw an exception with a value of 0
			} 
			// end of if/else

		} // end of for loop

	}// end of try
	catch(...)
	{
		cout<<"\nThe value you enter must be numeric."<<endl;
		system("pause");
		return false; // return confirming that value is not a valid number
	} // end catch

	return true; // return confirming tht the value is a valid number

} // end checkNumberFormat function

/********************
 * Currently Unused *
 ********************/
// function that add items to the stock of the specified type
void AddToStock(ITEM_SELECTED eItemUsed, float fItemAmount)
{
	switch(eItemUsed)
	{
		// if lemon was chosen increment the lemon stock and update the stock stucture array
		case Lemons:
			stock.lemons += fItemAmount;
			stockDays[data.stockPurchases].itemType = Lemons;
			stockDays[data.stockPurchases].itemName = "Lemons";
			stockDays[data.stockPurchases].daysLeft = TIME_LENGTH_LEMONS;
		break;

		// if sugar was chosen increment the sugar stock and update the stock stucture array
		case Sugar:	
			stock.sugarBags += fItemAmount;
			stock.sugarCups += (unsigned short)(fItemAmount * SUGAR_PER_BAG);	
			stockDays[data.stockPurchases].itemType = Sugar;
			stockDays[data.stockPurchases].itemName = "Sugar";
			stockDays[data.stockPurchases].daysLeft = TIME_LENGTH_SUGAR;
		break;

		// if ice was chosen increment the ice stock and update the stock stucture array
		case Ice: 
			stock.iceBags += fItemAmount;
			stock.iceCups += (unsigned short)(fItemAmount * ICE_CUPS_PER_BAG);
			stock.iceCubes += (unsigned short)(fItemAmount * ICE_CUBES_PER_CUP * ICE_CUPS_PER_BAG);

			stockDays[data.stockPurchases].itemType = Ice;
			stockDays[data.stockPurchases].itemName = "Ice";
			stockDays[data.stockPurchases].daysLeft = TIME_LENGTH_ICE;
		break;

		default: cout<<"\n An error occured with eChosen variable."<<endl;

	} // end of conditions

	// write the stock purchase entry information to the array
	stockDays[data.stockPurchases].quantity = fItemAmount;
	stockDays[data.stockPurchases].dayBought = data.dayNum;
	stockDays[data.stockPurchases].daysIn = 0;
	// set the day the item was purchased to the current day
	stockDays[data.stockPurchases].dayBought = data.dayNum;

	data.stockPurchases++;
		
} // end of AddToStock function

// function that disables or removes an entry from the stock purchase list
void RemoveFromStock(ITEM_SELECTED eItemUsed, float fRemoveAmount)
{	
	// local variables
	float fAmountLeft = fRemoveAmount; // the amount left that still needs to be removed
	unsigned short sIndex = 0; // the index in the array being used

	// remove the appropriate ammount of an item from the stock structure
	switch(eItemUsed)
	{
		case Lemons: 
			stock.lemons -= fRemoveAmount;
			if(DebugMode == true)
				cout<<"\nLemons"<<endl;
			break;

		case Sugar: 
			stock.sugarBags -= fRemoveAmount / SUGAR_PER_BAG;
			stock.sugarCups -= (unsigned short)fRemoveAmount;
			if(DebugMode == true)
				cout<<"\nSugar"<<endl;
			break;

		case Ice:
			stock.iceBags -= fRemoveAmount;
			stock.iceCubes -= (unsigned short)(fRemoveAmount * (ICE_CUPS_PER_BAG*ICE_CUBES_PER_CUP));
			if(DebugMode == true)
				cout<<"\nIce"<<endl;
			break;

		default:
			cout<<"Error: An invalid item was passed to function 'RemoveFromStockList'"<<endl;
			system("pause");

	} // end of switch statement

	for(int i=0; i<data.stockPurchases; i++) // iterate through the stock days array
	{	
		// if there is a value in the index and it is the correct item
		if(stockDays[i].itemType == eItemUsed)
		{
			// if there is less in the stock index than the amount required
			if(stockDays[i].quantity <= fAmountLeft)
			{
				// increase the amount removed by the amount in the index
				fAmountLeft -= stockDays[i].quantity;

				for(int j=i; j<data.stockPurchases; j++)
				{
					stockDays[j] = stockDays[j+1];
				} // end of inner for loop				

				// decrement the number of individual stock purchases made
				data.stockPurchases--;
				i--; // go back to the previous index in the stock list

				if(DebugMode == true)
					cout<<"Amount Left: "<<fAmountLeft<<endl;
				// end of if
			}
			else
			{				
				// remove the appropriate amount from the entry depending on how much still needs to be used
				stockDays[i].quantity -= fAmountLeft;
				fAmountLeft = 0; // all required stock has been removed

				if(DebugMode == true)
					cout<<"Amount Left: "<<fAmountLeft<<endl;

			} // end of inner if/else statement

		} // end of outter if
		
		if(fAmountLeft <= 0) // if enough stock has been removed
			return; // exit the function
		// end of if

	} // end of outter for loop

	// display an error if array is exceeded
	cout<<"Error: Not enough ";	
	switch(eItemUsed)
	{
		case Lemons: cout<<"Lemons";
			break;
		case Sugar: cout<<"Sugar";
			break;
		case Ice: cout<<"Ice";
			break;
		default:
			cout<<"Error: An invalid item was passed to function 'RemoveFromStockList'"<<endl;
			system("pause");
	}		
	cout<<" can be found in the stock purchase list."<<endl;

} // end of funcuntion