/* Debug.cpp
 * Programmer: Paul King
 * Last Updated: 4/11/2010
 * Description: This file is where all the functions for the debug menu and debug tools are
 *				located. A user can have more control over the program here for testing and debug
 *				purposes by using options found on the ebdug screens.
 *				Function prototypes, external variables and objects and other header files that
 *				this source file uses can be found in the "MainGameLoop.h" header file.
*/

#include "Debug.h"

// declare global variables
float fAmountEntered; // the floating point number entered by the user

// function the allows acces to ebug tools
void RunDebugMenu()
{
	// declare local variable
	float sIndexChosen;

	system("cls");

	do
	{
		cout<<"Debug Menu\n"
			<<"----------\n"
			<<"(1) Change Player Money\n"
			<<"(2) Add Stock\n"
			<<"(3) Remove Stock\n"
			<<"(4) Set Stock Amount\n"
			<<"(5) Delete Stock List Entries\n"
			<<"(6) Change Winning Total\n"
			<<"(7) Change Weather Forecast\n"
			<<"(0) Exit\n"
			<<"Select an option: ";
		cInput = getchar();
		fflush(stdin);

		switch(cInput)
		{
			case '1': SetPlayerMoney();				
				break;
			case '2': ChangeStockItem(1);
				break;
			case '3': ChangeStockItem(2);
				break;
			case '4': ChangeStockItem(3);
				break;

			case '5':
				system("cls");
				do
				{		
					ShowStockList();

					cout<<"\n(1) Remove an entry\n"
						<<"(2) Exit\n"
						<<"Select yout option: ";
					cInput = getchar();

					if(cInput == '1')
					{
						cout<<"Enter which entry you want to remove from stock: ";
						cin>>sIndexChosen;						

						cout<<"The select entry has been deleted from the stock list."<<endl;
					}
					else if(cInput == '2')
					{
						return;
					}
					else
					{
						cout<<"\nThat was not a valid response."<<endl;
					}
					// end of if/else statements

					PauseAndClear();
					fflush(stdin);

				} while(cInput!= '2'); // end of do/while loop
			break;

			case '6':
				cout<<"Enter the winning money total total: ";
				cin>>data.targetMoney;

				cout<<"The winning amount has been changed."<<endl;
				PauseAndClear();
			break;

			case '7': SetWeatherConditions();
				break;

			case '0':
				break;

			default: 
				cout<<"\nThat was not a valid response."<<endl;
				PauseAndClear();

		} // end of switch

		fflush(stdin);
		//system("cls");

	} while(cInput != '0');
	// end of do/while loop

} // end of RunDebugMenu function

// function that allows user to change money total in debug menu
void SetPlayerMoney()
{
	system("cls");
	do
	{		
		cout<<"Money Menu\n"
			<<"----------\n"
			<<"(1) Set Money Total\n"
			<<"(2) Add Money\n"
			<<"(3) Remove Money\n"
			<<"(4) Exit\n"
			<<"Select an option: ";
		cInput = getchar();
		fflush(stdin);

		switch(cInput)
		{
			case '1':
				cout<<"Set new player money amount: ";
				cin>>data.currentMoney;
				cout<<"The player's money has been changed."<<endl;
				PauseAndClear();
			break;

			case '2': 
				cout<<"Set amount to add to current funds: ";
				cin>>fAmountEntered;
				// increase player money by the amount entered
				data.currentMoney += fAmountEntered;
				cout<<"The player's money has been changed."<<endl;	
				PauseAndClear();
			break;

			case '3':
				cout<<"Set amount to remove from current funds: ";
				cin>>fAmountEntered;
				// decrease player money by the amount entered
				data.currentMoney -= fAmountEntered;
				cout<<"The player's money has been changed."<<endl;
				PauseAndClear();
			break;

			case '4': system("cls");
				break;

			default:					
				cout<<"\nThat was not a valid response."<<endl;
				PauseAndClear();

		} // end of switch statement

		fflush(stdin);

	} while(cInput != '4'); // end of do/while loop

} // end of SetPlayerMoney function

// function that changes the quantity of a specific item type in stock
void ChangeStockItem(unsigned short optionType)
{
	// declare local variables
	ITEM_SELECTED eItemChosen; // enumerator variable for stock item type chosen
	float fAmount; // the amount to add, subtract or change the stock amount to

	cout<<"Option Entered: "<<optionType<<endl;

	// prompt user for stock item type to change
	cout<<"Which stock item do you want to change the value of?\n"
		<<"(1) - Lemons, (2) - Sugar Bags, (3) - Ice Bags, (4) - Lemonade Jugs, (5)-Cancel\n"
		<<"Enter your option: ";
	cInput = getchar();

	// convert the character entered into enumerator value
	eItemChosen = (ITEM_SELECTED)(cInput-49);

	if(optionType == 1)
	{
		// add the requested amount of requested item from stock
		switch(eItemChosen)
		{
			case Lemons: 
				cout<<"Enter how many lemons you want to add: ";
				cin>>fAmount;
				AddToStock(Lemons, fAmount);
				break;

			case Sugar: 
				cout<<"Enter how many bags of sugar you want to add: ";
				cin>>fAmount;
				AddToStock(Sugar, fAmount);		
				break;

			case Ice: 
				cout<<"Enter how many bags of ice you want to add: ";
				cin>>fAmount;
				AddToStock(Ice, fAmount);
				break;

			case Lemonade:
				cout<<"Enter how many jugs of lemonade you want to add: ";
				cin>>fAmount;
				stock.lemonadeJugs += fAmount;
				stock.lemonadeCups += (unsigned short)(fAmount*CUPS_PER_JUG);
			break;

			case Exit_Screen: 
				system("cls");
				return; // leave function immediataely


			default:
				cout<<"Error: An invalid item was found. Value="<<eItemChosen<<endl;

		} // end of switch statement

	} // end of option 1
	else if(optionType == 2)
	{
		if(eItemChosen != Exit)
		{			
			cout<<"Enter how much of the selected item you want to remove: ";
			cin>>fAmount;
		} // end of if		

		// remove the requested amount of requested item from stock
		switch(eItemChosen)
		{
			case Lemons: 
				if(stock.lemons >= fAmount)
				{
					RemoveFromStock(Lemons, fAmount);
					cout<<"The requested stock item has been updated."<<endl;
				}
				else
				{
					cout<<"There isn't enough lemons in stock to remove."<<endl;
				} // end of if/else
			break;

			case Sugar:
				if(stock.sugarBags >= fAmount)
				{
					RemoveFromStock(Sugar, fAmount);	
					cout<<"The requested stock item has been updated."<<endl;
				}
				else
				{
					cout<<"There isn't enough sugar in stock to remove."<<endl;
				} // end of if/else
			break;

			case Ice:
				if(stock.iceBags >= fAmount)	
				{
					RemoveFromStock	(Ice, fAmount);	
				}
				else
				{
					cout<<"There isn't enough ice in stock to remove."<<endl;
				} // end of if/else
			break;

			case Lemonade:
				if(stock.iceBags >= fAmount)
				{
					stock.lemonadeJugs -= fAmount;
					stock.lemonadeCups -= (unsigned short)(fAmount*CUPS_PER_JUG);
				}
				else
				{
					cout<<"There isn't enough lemonade in stock to remove."<<endl;
				}
				// end of if/else
			break;

			case Exit_Screen: 
				return; // leave function immediataely


			default:
				cout<<"Error: An invalid item was found. Value="<<eItemChosen<<endl;

		} // end of switch statement
	}
	else if(optionType == 3)
	{
		cout<<"Enter how much of the selected you want in stock: ";
		cin>>fAmount;

		// change the requested amount of requested item from stock
		switch(eItemChosen)
		{
			case Lemons: stock.lemons = fAmount;
				break;

			case Sugar:
				stock.sugarBags = fAmount;
				stock.sugarCups = (unsigned short)(fAmount*SUGAR_PER_BAG);
			break;

			case Ice:
				stock.iceBags = fAmount;
				stock.iceCubes = (unsigned short)(fAmount*SUGAR_PER_BAG);
			break;

			case Lemonade:
				stock.lemonadeJugs = fAmount;
				stock.lemonadeCups = (unsigned short)(fAmount*CUPS_PER_JUG);
			break;

			case Exit_Screen: return; // leave function immediataely
				break;

			default:
				cout<<"Error: An invalid item was found. Value="<<eItemChosen<<endl;

		} // end of switch statement

	} // end of option 3
	else
	{
		cout<<"Error: The option passed to the 'ChangeStockItem' was invalid. Value="<<optionType<<endl;

	} // end of option 4

	cout<<"The stock item amount has been updated."<<endl;

	PauseAndClear();

} // end of function

// function that changes the weather conditions to ones specified by the user
void SetWeatherConditions()
{
	//local variables
	WEATHER_OVERCAST overcast;// enumerator variable for the overcast conditions
	bool blValidation; // checks that the entry in numerical
	char cEntered[16];
	unsigned short temperatureEntered; // the temperature selected by the user

	try
	{
		do
		{
			// prompt user for temperature
			cout<<"Min("<<TEMPERATURE_MIN<<"o), Max("<<TEMPERATURE_MAX<<"o)\n"
				<<"Enter the temperature in degrees celcius: ";
			cin.getline(cEntered, 16);
			//cin.ignore();
			
			blValidation = CheckEntryFormat(cEntered); // checks that the value entered is numerical

			if(blValidation == false)
				throw 0;
			// end of if

			temperatureEntered = atoi(cEntered); // convert the string to a number for the amount entered variable
		
			// if the temperature entered was not within range confirm so
			if((temperatureEntered < TEMPERATURE_MIN) || (temperatureEntered > TEMPERATURE_MAX))
			{
				cout<<"The amount you entered was not within the minuman and maximum\n"
					<<"Temperature range";
			}
			else // otherwise change it
			{
				weather.tempCel = temperatureEntered;
				weather.tempFah = (unsigned short)(weather.tempCel * 1.8 + 32);					
			}
			// end of if/else statement
		} while((temperatureEntered < TEMPERATURE_MIN) || (temperatureEntered > TEMPERATURE_MAX));

		// get the temperature codintion depending on the temperature in degrees celcius
		if(weather.tempCel < 20)
			weather.tempRange = Cold;
		else if((weather.tempCel >= 20) && (weather.tempCel < 30))
			weather.tempRange = Warm;
		else if((weather.tempCel >= 30) && (weather.tempCel < 40))
			weather.tempRange = Hot;
		else if(weather.tempCel > 40)
			weather.tempRange = Very_Hot;
		// end of if/else conditions

		// prompt user for overcast conditions
		cout<<"(1) - No Clouds, (2) - Few Clouds, (3) - Many Clouds, (4) - Cloudy\n"
			<<"Select your overcaset conditions: ";
		cInput = getchar();
		overcast = (WEATHER_OVERCAST)(cInput-47);
		
		// if an invalid choice was made say so
		if((fAmountEntered < 1) || (fAmountEntered > 4))
		{
			cout<<"That selection was invalid.\n";
		}
		else // otherwise change the overcast conditions
		{
			weather.overcast = overcast;

		} // end of if/else conditions	

		if((weather.tempCel == Cold) && (weather.overcast == Full_Overcast))
		{
			do
			{
				// ask user whether to set rain for weather
				cout<<"Do you want to set rain for todays weather? (Y)es (N)o."
					<<"Select an option: ";
				cInput = getchar();
				fflush(stdin);

				if((cInput == 'Y') || (cInput == 'y'))
					weather.rain = true;
				else if((cInput == 'N') || (cInput == 'n'))
					weather.rain = false;
				else
					cout<<"That was no a valid response. Please try again.\n"<<endl;
				// end of if/else conditions

			} while((cInput != 'Y') && (cInput != 'y') && (cInput != 'N') && (cInput != 'n'));
			// end of do/while loop

		} // end of if

		cout<<"Today's weather conditions have been changed."<<endl;

	} // end of try
	catch(...)
	{
		cout<<"The number you have entered must be numerical."<<endl;
	} // end of catch statement

	PauseAndClear();

} // end of SetWeatherConditions function