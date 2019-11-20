/* Lemonade.cpp
 * Programmer: Paul King
 * Last Updated: 4/11/2010
 * Description: This file holds the functions for most menus the are opened beyond the in-game
 *				menu when the game is running. The function are all called within
 *				'MainGameLoop.cpp' or this file. Looking at the finances, seeing stock purchases,
 *				buying stock, changing the recipe or price and making lemonade are done within
 *				the functions located in this file.
 *				Function prototypes, external variables and objects and other header files that
 *				this source file uses can be found in the "MainGameLoop.h" header file.
*/

//Include Header File
#include "Lemonade.h"

/*************
 * Functions *
 *************/
// function the displays the finances screen
void SeeFinances()
{
	// calculate the total profit
	financeTotal.totalProfit = financeTotal.moneyMade - financeTotal.moneySpent;

	system("cls"); // clear the screen

	// diplay the finances to the player
	cout<<"\t  "<<data.playerName<<"'s Finances\n"
		<<"\t  ----------------------"<<endl;

	DisplayStockAndPrices();

	cout<<"Stock Loss\n"
		<<"----------\n"
		<<"Lemons Gone Off: "<<financeTotal.lemonsOff<<endl
		<<"Bags Of Sugar Infested By Rodents: "<<financeTotal.sugarInfestRodents<<" ("
		<<financeTotal.sugarInfestRodents*SUGAR_PER_BAG<<" Cups)\n"
		<<"Cups Of Sugar Infested By Ants: "<<financeTotal.sugarInfestAnts<<" ("
		<<financeTotal.sugarInfestAnts*SUGAR_PER_BAG<<" Bags)\n"
		<<"Bags Of Ice Melted: "<<financeTotal.iceMelted<<" ("
		<<financeTotal.iceMelted*ICE_CUPS_PER_BAG*ICE_CUBES_PER_CUP<<" Cubes\n"
		<<"Cups of lemonade lost due to accidents and free produce: "<<financeTotal.cupsDestroyed
		<<endl<<endl;

	cout<<"Sales Info\n"
		<<"----------\n"	
		<<"Total Lemonade Cups Sold: "<<financeTotal.cupsSold<<" ("<<financeTotal.jugsSold<<" Jugs)\n"
		<<"Total Lemonade Cups Not Sold: "<<financeTotal.cupsLost<<" ("<<financeTotal.jugsLeftover<<" Jugs)\n"
		<<endl
		<<"Money Info\n"
		<<"----------\n";
	printf("Money Made From Sales: $%.2f\n", financeTotal.moneyMade);
	printf("Money Spent On Stock: $%.2f\n", financeTotal.moneySpent);
	printf("Total Profit: $%.2f\n\n", financeTotal.totalProfit);

	printf("Current Funds: $%.2f\n\n", data.currentMoney);		

	PauseAndClear();

} // end of SeeFinances function

// function the displays the current stock
void DisplayStockItems()
{	
	cout<<"Items in stock\n"
		<<"--------------\n"
		<<"Lemons: "<<stock.lemons<<endl
		<<"Bags of sugar: "<<stock.sugarBags<<endl
		<<"Bags of Ice: "<<stock.iceBags<<endl
		<<"Jugs of Lemonade: "<<stock.lemonadeJugs<<endl
		<<"Cups of Lemonade: "<<stock.lemonadeCups<<endl<<endl;

} // end of display stock items function

// function that displays the current stock with the prices
void DisplayStockAndPrices()
{
	// create local variable for the number of ice cubes in a bag
	unsigned short sIceCubesPerBag = ICE_CUPS_PER_BAG * ICE_CUBES_PER_CUP;

	cout<<"Current Stock\t\t    Item Prices\n"
		<<"-------------\t\t    -----------"<<endl;
	printf("Lemons: %.2f\t\t    ",stock.lemons);
	printf("$%.2f per kilo (1kg = %i lemons)\n", prices.lemons, LEMONS_PER_KILO);
	printf("Bags of sugar: %.2f\t    ", stock.sugarBags);
	printf("$%.2f per bag of sugar (1 bag = %i cups)\n", prices.sugarBags, SUGAR_PER_BAG);
	cout<<"Bags of Ice: "<<stock.iceBags<<"\t\t    ";
	printf("$%.2f per bag of ice (1 bag = %i cups or %i cubes)\n\n", 
		   prices.iceBags, ICE_CUPS_PER_BAG, sIceCubesPerBag);

} // end of DisplayStockAndPrices function

// function that displays the current recipe being used
void DisplayRecipe()
{
	// declare local variables
	float fLemonsPerCup, fSugarPerCup, fIcePerCup; // the recipe for each cup of lemonade
	float fRecipeSugarBags, // the amount of sugar bags used in the recipe
		  fRecipeIceBags; // the amount of ice bags used in the recipe		  

	unsigned short sIceCupsUsed;
	
	// calculate the ice cubes used
	sIceCupsUsed = recipe.ice * ICE_CUBES_PER_CUP;

	// calculate the bags used for sugar and ice in the recipe
	fRecipeSugarBags = recipe.sugar / SUGAR_PER_BAG;
	fRecipeIceBags = (float)recipe.ice / ICE_CUPS_PER_BAG;

	// calculate items per each cup
	fLemonsPerCup = recipe.lemon / CUPS_PER_JUG;
	fSugarPerCup = recipe.sugar / CUPS_PER_JUG;
	fIcePerCup = (float)recipe.ice / CUPS_PER_JUG;

	// calculate how much is spent on each ingrediant for each cup of lemonade
	prices.lemonsPerCup = prices.lemons / LEMONS_PER_KILO * (recipe.lemon / CUPS_PER_JUG);
	prices.sugarPerCup = prices.sugarBags / SUGAR_PER_BAG * (recipe.sugar / CUPS_PER_JUG);
	prices.icePerCup = (float)recipe.ice / CUPS_PER_JUG * (recipe.ice / (float)CUPS_PER_JUG);

	// calculate the overall cost spent on each cup
	prices.totalPerCup = prices.lemonsPerCup + prices.sugarPerCup + prices.icePerCup;

	cout<<"Current Recipe\n"
		<<"--------------\n"
		<<"Recipe Per jug \t\t\t    Recipe Per Cup \t Price Per Cup\n"
		<<"-------------- \t\t\t    -------------- \t -------------\n"
		<<recipe.lemon<<" lemons   \t\t\t    ";
	printf("%.2f lemons \t     ",fLemonsPerCup);
	printf("$%.2f\n", prices.lemonsPerCup);

	cout<<recipe.sugar<<" cups of sugar ("<<fRecipeSugarBags<<" bags)\t    ";
	printf("%.2f cups of sugar \t     ", fSugarPerCup);
	printf("$%.2f\n", prices.sugarPerCup);

	cout<<recipe.ice<<" cups of ice ("<<fRecipeIceBags<<" bags)("<<sIceCupsUsed<<" cubes)  ";
	printf("%.2f cups of ice\t     ", fIcePerCup);
	printf("$%.2f\n", prices.icePerCup);
	printf("\t\t\t\t\t\t      Total: $%.2f\n\n", prices.totalPerCup);

} // end of DisplayRecipe function

// function that displays the stock screen and allows thepurchasing of new stock
void BuyStock()
{
	// do the following while the menu is running
	do 
	{
		system("cls"); // clear the screen

		/* call functions to display information about the current stock, price of stock 
		   and current recipe */
		DisplayStockAndPrices();
		DisplayRecipe();

		printf("Current Funds: $%.2f\n\n", data.currentMoney);

		// display the submenu
		cout<<"(1) Purchase individual stock items\n"
			<<"(2) Select stock automatically by lemonade amount\n"
			<<"(3) Exit\n"
			<<"Select an action: ";
		cInput = getchar(); // get first character entered as input
		fflush(stdin); // removes the character currently in buffer

		switch(cInput)
		{
			case '1':
				// call the function that runs the manual purchsing next submenu
				SelectStockManually();						
			break;

			case '2':
				// call the function the prompts user for cups to make and calculats stock
				SelectStockAutomatically();				
			break;

			case '3':
				break;

			default:
				cout<<"\nThat response was invalid."<<endl;
				PauseAndClear();

		} // end of switch statement

	} while(cInput != '3'); // end of do/while loop

	system("cls");

} // end of BuyStock function

// function the automatically picks neccesary stock items based on the amount of jugs made
void SelectStockAutomatically()
{
	// declare local variables
	char strInput[16]; // the string entered by the user
	unsigned short sJugsQuantity, // the amount of jugs  jugs to buy required stock for	
				   fNeededLemons, sNeededLemonKilos, sNeededSugarCups, sNeedediceCups, sNeededSugarBags,
				   sNeededIceBags; // quantity of items needed for the requested number of jugs
	// the price of the purchase for each item and the grand total
	float fPriceLemons, fPriceSugar, fPriceIce, fPriceTotal;

	cout<<"\nEnter the number of lemonade jugs you would like to produce (0 to cancel): ";
	cin.getline(strInput, 15);	

	// call function that checks for exceptions and hanleds them
	blValidation = CheckEntryFormat(strInput);
	// convert the string to an integer to pass to sItemQuantity
	sJugsQuantity = atoi(strInput);

	if(sJugsQuantity != 0)
	{
		// calculate how many of each of the three ingredients rounded to integers will be required
		fNeededLemons = (unsigned short) (sJugsQuantity * recipe.lemon);
		sNeededSugarCups = (unsigned short) (sJugsQuantity * recipe.sugar);
		sNeedediceCups = (unsigned short) (sJugsQuantity * recipe.ice);

		// calculate how many bags will be needed based on the number of cups per bag
		sNeededLemonKilos = (unsigned short) ceil(sJugsQuantity * (recipe.lemon / LEMONS_PER_KILO));
		sNeededSugarBags = (unsigned short) ceil(sJugsQuantity * (recipe.sugar / SUGAR_PER_BAG));
		sNeededIceBags = (unsigned short) ceil(sJugsQuantity * ((float)recipe.ice / ICE_CUPS_PER_BAG));

		// calcuate the total prices of each item and the grand total
		fPriceLemons = sNeededLemonKilos * prices.lemons;
		fPriceSugar = sNeededSugarBags * prices.sugarBags ;
		fPriceIce = sNeededIceBags * prices.iceBags;
		fPriceTotal = fPriceLemons + fPriceSugar + fPriceIce;

		// Tell user the stock quantity and the prices
		cout<<"\nYou have chosen to make "<<sJugsQuantity<<" jugs of lemonade.\n"
			<<"To do this you will require the following ingredients..."<<endl;
		printf("%i Lemons (%ikg)($%.2f)\n", fNeededLemons, sNeededLemonKilos, fPriceLemons);
		printf("%i bags of sugar (%i sugar cups)($%.2f)\n", sNeededSugarBags, sNeededSugarCups, fPriceSugar);
		printf("%i bags of ice (%i ice cubes)($%.2f)\n\n", sNeededIceBags, sNeedediceCups, fPriceIce);
		printf("This will cost a total of $%.2f\n", fPriceTotal);
		
		do
		{
			// ask user to confirm the purchase
			cout<<"Make this purchase? (1) Yes, (2) No: ";
			cInput = getchar();
			fflush(stdin);

			if(cInput == '1')
			{
				if(data.currentMoney >= fPriceTotal)
				{
					// call function to process the purchases
					ProcessPurchase(Lemons, fNeededLemons, fPriceLemons);
					ProcessPurchase(Sugar, sNeededSugarBags, fPriceSugar);
					ProcessPurchase(Ice, sNeededIceBags, fPriceIce);			

					cout<<"\nThe required stock items to make lemonade have been added to the stock.\n"
						<<"Note: Lemonade jugs will not be filled until you go to the make lemonade screen."<<endl;
				}
				else
				{
					cout<<"\nYou do not have enough money to afford this purchase."<<endl;
				}
				// end of inner if/else
			}
			else if(cInput != '2')
			{
				cout<<"That response was not valid."<<endl<<endl;
			}
			// end of middle if/else

			system("pause");

		} while((cInput != '1') && (cInput != '2'));
		// end of do/while loop

	} // end of if for sJugsToMake

} // end of SelectStockAutomtically function

// function that handles the decision made on the buy stock screen
void SelectStockManually()
{
	// character array for input
	char cStringInput[16];		

	// declare local variables
	unsigned short sItemQuantity; // the quanity chosen to buy
	//short sDimensionUsed; // the dimension of the array being used to add items to
	float fTotalPrice; // the price of all items
	ITEM_SELECTED eChosen; // the item chosen to purchase

	// create string array
	char* cItemName[] = {"kg of lemons.", " bags of sugar.", " bags of ice."};

	system("cls");

	do
	{
		// call functions to display screen information again
		DisplayStockAndPrices();
		DisplayRecipe();

		printf("Current Funds: $%.2f\n\n", data.currentMoney);
	
		// display submenu
		cout<<"(1) Buy Lemons\n"
			<<"(2) Buy Sugar Bags\n"
			<<"(3) Buy Ice Bags\n"
			<<"(4) Cancel\n"
			<<"Select an action: ";
		cInput = getchar(); // get first character entered as input
		fflush(stdin); // removes the character currently in buffer

		switch(cInput) // perform cases for character entered
		{
			case '1': case '2':	case '3':
				eChosen = (ITEM_SELECTED)(cInput-49);
				// end of if/else statement
			break;

			case '4': 
				return; // will exit the while loop in buy stock function
				break;

			default: 
				cout<<"That response was not valid."<<endl;
				system("pause");
				return; // return to the line after this function was called
		} // end of switch

		do
		{
			cout<<"How many do you wish to purchase? (0 to cancel): ";
			cin>>cStringInput;

			fflush(stdin); // clear the buffer of all text

			// call function that checks for exceptions
			blValidation = CheckEntryFormat(cStringInput);			

			// convert the string to an integer to pass to sItemQuantity
			sItemQuantity = atoi(cStringInput);

			// the amount chosen to buy is less than 0 or greater than 999
			if((sItemQuantity < 0) || (sItemQuantity > 999))
			{
				cout<<"You must choose to buy between 0 and 999 of the stock item"<<endl<<endl;			
				blValidation = false;

			} // end of if			

		}while(blValidation == false); // repeat if the number entered was invalid

		if(sItemQuantity != 0)
		{
			switch(eChosen)
			{
				// if lemons were chosen multiply the cost per lemon by the amount bought
				case Lemons:
					fTotalPrice = prices.lemons * sItemQuantity;
					break;
				// if sugar was chosen multiply the cost per sugar by the amount bought
				case Sugar:
					fTotalPrice = prices.sugarBags * sItemQuantity;
					break;
				// if ice was chosen multiply the cost per ice bag by the amount bought
				case Ice:
					fTotalPrice = prices.iceBags * sItemQuantity;
					break;
			} // end of switch

			// display cost and prompt user to confirm purchase
			cout<<"\nYou have chosen to buy "<<sItemQuantity<<cItemName[eChosen]<<endl;
			printf("This will cost $%.2f\n", fTotalPrice);
			cout<<"(1) Yes, (2) No\n"
				<<"Confirm purchase?: ";	
			cInput = getchar();	
			fflush(stdin);

			if(cInput == '1')
			{
				if(eChosen == Lemons)
					sItemQuantity *= LEMONS_PER_KILO;
				// end of if

				if(data.currentMoney >= fTotalPrice)
				{
					ProcessPurchase(eChosen, sItemQuantity, fTotalPrice);
					cout<<"The selected item(s) have been added to the stock."<<endl;
				}
				else
				{
					cout<<"\nYou do not have enough money to afford this purchase."<<endl;
				} // end of if/else
			}
			// end of if
			else if(cInput != '2')
			{
				cout<<"\nThat response was not valid"<<endl;
			}
			// end of else if

		} // end of if for sItemQuantity

		PauseAndClear();

	} while((cInput != '1') && (cInput != '2') && (cInput != '3') && (cInput != '4'));
	// end of do/while loop

} // end of buyItem function

// function the handles the requested amount of item chosen to buy of a particular type
void ProcessPurchase(ITEM_SELECTED itemChosen, float quantity, float totalPrice)
{
	switch(itemChosen)
	{
		// if lemon was chosen increment the lemon stock and update the stock stucture array
		case Lemons: stock.lemons += quantity;
			break;

		// if sugar was chosen increment the sugar stock and update the stock stucture array
		case Sugar:	
			stock.sugarBags += quantity;
			stock.sugarCups += quantity * SUGAR_PER_BAG;
		break;

		// if ice was chosen increment the ice stock and update the stock stucture array
		case Ice: 
			stock.iceBags += quantity;
			stock.iceCups += quantity * ICE_CUPS_PER_BAG;
			stock.iceCubes += (unsigned short)(quantity * ICE_CUPS_PER_BAG * ICE_CUBES_PER_CUP);
		break;

		default: cout<<"\n An error occured with eChosen variable."<<endl;

	} // end of conditions

	data.currentMoney -= totalPrice; // subtract the money by the total price paid 

	financeDaily.moneySpent += totalPrice; // add the price to the total money spent today
	financeTotal.moneySpent += totalPrice; // add the price to the total money spent overall

} // end of Process Purchase function

// function that displays the recipe & price screen and allows user to change it
void SetRecipeOrPrice()
{
	// declare local variables
	bool blMenuRunning = true; // determines whether the loop will continue
	// enumerator variables
	ITEM_SELECTED eChosen;
	RECIPE_SWEETNESS eRecipeTaste; // the sweet and sour level of the recipe
	RECIPE_TEMPERATURE eRecipeTempureture; // how hot or cold the lemonade is

	char cStringEntered[5];
	
	system("cls"); // clear the screen

	do
	{
		//if(DebugMode == true)
		//{
			// call functions from customer.cpp to determine the recipe and temperature condition
			eRecipeTaste = GetRecipeSweetness();		
			eRecipeTempureture = GetRecipeTemperature();

			cout<<"Recipe Taste Level: ";
			switch(eRecipeTaste)
			{
				case Very_Sour: cout<<"Very Sour"<<endl;
					break;
				case Sour: cout<<"Sour"<<endl;
					break;
				case Slightly_Sour: cout<<"Slightly Sour"<<endl;
					break;
				case Average: cout<<"Average"<<endl;
					break;
				case Slightly_Sweet: cout<<"Slightly Sweet"<<endl;
					break;
				case Sweet: cout<<"Sweet"<<endl;
					break;
				case Very_Sweet: cout<<"Very Sweet"<<endl;
					break;

			} // end if switch statement

			cout<<"Recipe Temperature Level: ";
			switch(eRecipeTempureture)
			{
				case Very_Cold: cout<<"Very Cold"<<endl;
					break;
				case Somewhat_Cold: cout<<"Somewhat Cold"<<endl;
					break;
				case Slightly_Cold: cout<<"Slightly Cold"<<endl;
					break;
				case Ideal: cout<<"Ideal"<<endl;
					break;
				case Slightly_Warm: cout<<"Slightly Warm"<<endl;
					break;
				case Somewhat_Warm: cout<<"Somewhat Warm"<<endl;
					break;
				case Very_Warm: cout<<"Very Warm"<<endl;
					break;

			} // end of switch statement

			printf("Customer Purchase Max Price Range: $%.2f - $%.2f \n\n", BASE_MAX_PRICE, BASE_MAX_PRICE+1);

		//} // end of if

		DisplayRecipe(); // displays the current recipe for each jug and cup		
		
		// display the sale price
		printf("Sale Price: $%.2f\n\n", prices.lemonadePrice);
			
		cout<<"(1) Change Lemon Recipe\n"
			<<"(2) Change Sugar Recipe\n"
			<<"(3) Change Ice Recipe\n"
			<<"(4) Change Price\n"
			<<"(5) Exit\n"
			<<"Select an action: ";
		cInput = getchar();
		fflush(stdin);
		
		eChosen = (ITEM_SELECTED)(cInput-49);
		
		switch(eChosen)
		{
			case Lemons: case Sugar: case Ice:
				
			if(stock.lemonadeJugs <=0)
			{
				if(eChosen == Lemons) // the lemon recipe was chosen
				{
					cout<<"Enter the number of lemons per jug: ";
					cin>>cStringEntered;
					fflush(stdin); // clear the buffer of all text entered;

					// call function that changes the value of the lemons in the recipe
					recipe.lemon = UpdateItemValue(Lemons, cStringEntered, recipe.lemon);
				}
				else if(eChosen == Sugar)  // the sugar recipe was chosen
				{
					cout<<"Enter the number of cups of sugar per jug: ";
					cin>>cStringEntered;
					fflush(stdin);

					// call function that changes the value of the lemons in the recipe	
					recipe.sugar = UpdateItemValue(Sugar, cStringEntered, recipe.sugar);			
				}
				else if(eChosen == Ice)
				{			
					cout<<"Enter the number of cups of ice per jug: ";
					cin>>cStringEntered;
					fflush(stdin);

					recipe.ice = (unsigned short) (UpdateItemValue(Ice, cStringEntered, recipe.ice));							
				}
				// end of inner if/else statements

			} // end of outter if
			else
			{
				cout<<"You can not change the recipe once you already have jugs of\n"
					<<"lemonade filled. Come back here the next day."<<endl;
				PauseAndClear();
			} // end of outter if/else statement

			break;

			case Lemonade:				
				cout<<"Enter the new sale price: $";
				cin>>cStringEntered;
				fflush(stdin);
					
				prices.lemonadePrice = UpdateItemValue(Lemonade, cStringEntered, prices.lemonadePrice);				
			break;

			case Exit_Screen:
				system("cls");
			break;

			default:
				cout<<"\nThat was not a valid response\n";
				PauseAndClear();

		} // end of switch		

	} while(eChosen != Exit_Screen); // end of do/while loop

} // end of SerRecipeOrPrice function

// function that changes the value of the recipe item or price once the user enters their decision
float UpdateItemValue(ITEM_SELECTED itemChosen, char stringFound[], float defaultValue)
{
	// declare locale variable for the amount entered
	float fAmountChosen;

	// call function that checks for a number exception in text entry
	blValidation = CheckEntryFormat(stringFound);
				
	if(blValidation == true) // if a valid number was found
	{
		// convert the string to a floating point number for float variable
		fAmountChosen = (float) atof(stringFound); 

		switch(itemChosen)
		{
			case Lemons:
				if(fAmountChosen < RECIPE_MIN_LEMONS)
				{
					cout<<"You must have atleast "<<RECIPE_MIN_LEMONS<<" lemons in your recipe"<<endl;
					PauseAndClear();
					return recipe.lemon;
				}
				else if(fAmountChosen > RECIPE_MAX_LEMONS)
				{
					cout<<"The maximum amount of lemons you can have is "<<RECIPE_MAX_LEMONS<<endl;
					PauseAndClear();
					return recipe.lemon;
				}
				else
				{				
					cout<<"\nThe lemons amount in your recipe has been updated."<<endl;
				}
				// end of if/else conditions
			break;

			case Sugar:
				if(fAmountChosen < RECIPE_MIN_SUGAR)
				{
					cout<<"You must have atleast "<<RECIPE_MIN_SUGAR<<" cups of sugar in your recipe."<<endl;
					PauseAndClear();
					return recipe.sugar;
				}
				else if(fAmountChosen > RECIPE_MAX_SUGAR)
				{
					cout<<"The maximum cups of sugar you can have in you recipe is "<<RECIPE_MAX_SUGAR<<endl;
					PauseAndClear();
					return recipe.sugar;
				}
				else
				{				
					cout<<"\nThe sugar amount in your recipe has been updated."<<endl;
				}
				// end of if/else conditions
			break;

			case Ice:
				if(fAmountChosen < RECIPE_MIN_ICE)
				{
					cout<<"You must have atleast "<<RECIPE_MIN_ICE<<" ice cubes in your recipe"<<endl;
					PauseAndClear();
					return recipe.ice;
				}
				else if(fAmountChosen > RECIPE_MAX_ICE)
				{
					cout<<"The maximum number of ice cubes you can have in your recipe is "<<RECIPE_MAX_ICE<<endl;
					PauseAndClear();
					return recipe.ice;
				}
				else
				{				
					cout<<"\nThe ice amount in your recipe has been updated."<<endl;
				}
				// end of if/else conditions
			break;

			case Lemonade:
				if(fAmountChosen < PRICE_LEMONADE_MIN)
				{
					printf("The minimum price you can set is $.2f\n",PRICE_LEMONADE_MIN);
					PauseAndClear();
					return prices.lemonadePrice;
				}
				else if(fAmountChosen > PRICE_LEMONADE_MAX)
				{
					printf("The maximum price you can set is $.2f\n",PRICE_LEMONADE_MAX);
					PauseAndClear();
					return prices.lemonadePrice;
				}
				else
				{				
					cout<<"\nThe sale price of lemonade has been updated."<<endl;
				}
				// end of if/else conditions				
			break;

		} // end of switch statment

		PauseAndClear();
		return fAmountChosen;

	} // end outter if
	else
	{
		return defaultValue;
	} // end of outter else	
	
} // end of update value function

// function that goes to the make lemonade screen which allows the player to flll jugs
void MakeLemonade()
{	
	// declare local variables
	unsigned short sJugsToMake, // the amount of jugs chosen to make either by the computer or the user
				   // the maximum lemonade that can be made with specific sotck items
				   sMaxJugsLemons, sMaxJugsSugarCups, sMaxJugsIceCups;
	float fLossLemons, fLossSugerBags, fLossIceBags;

	bool blMenuRunning = true; // determines if the player will stay on this screen

	char cStringEntered[3]; // the string entered by the user

	system("cls"); // clear the scrren

	do
	{
		DisplayStockItems(); // call function that displays the stock on screen
		DisplayRecipe(); // call function that displays the current recipe

		// display menu
		cout<<CUPS_PER_JUG<<" cups of lemonade can be made from 1 jug"<<endl<<endl
			<<"(1) Fill Up Maxium Lemonade Jugs\n"
			<<"(2) Select Quantity Of Jugs To Fill\n"
			<<"(3) Exit\n"
			<<"Enter you action: ";
		cInput = getchar();
		fflush(stdin);

		switch(cInput)
		{
			case '1': case '2':

				// get the random amount of stock loss for lemons sugar and ice when lemonade is made
				fLossLemons = GetRandomStockLoss(Lemons);
				fLossSugerBags = GetRandomStockLoss(Sugar);
				fLossIceBags = GetRandomStockLoss(Ice);

				// tell user the stock loss
				if(fLossLemons > 0)
					cout<<fLossLemons<<" lemons went off"<<endl;
				if(fLossSugerBags > 0)
					cout<<fLossSugerBags<<" bags of sugar were infested by a rodent"<<endl;
				if(fLossIceBags > 0)
					cout<<fLossIceBags<<" Bags of ice melted"<<endl;
					
				cout<<endl;

				// remove the required items from stock
				stock.lemons -= fLossLemons;
				stock.sugarBags -= fLossSugerBags;
				stock.sugarCups -= fLossSugerBags * SUGAR_PER_BAG;
				stock.iceBags -= fLossIceBags;
				stock.iceCups -= fLossIceBags * ICE_CUPS_PER_BAG;

				if(cInput == '1')
				{					
					// calculate the maximum jugs that can be made with the ingredients based on the recipe
					sMaxJugsLemons = (unsigned short) floor(stock.lemons / recipe.lemon);
					sMaxJugsSugarCups = (unsigned short) floor(stock.sugarCups / recipe.sugar);
					sMaxJugsIceCups = (unsigned short) floor(stock.iceCups / recipe.ice);

					if(DebugMode == true)
					{
						cout<<"Stock\n"
							<<"-----\n"
							<<"Lemons: "<<stock.lemons<<endl
							<<"Sugar: "<<stock.sugarBags<<" bags, "<<stock.sugarCups<<" cups"<<endl
							<<"Ice: "<<stock.iceBags<<" bags, "<<stock.iceCups<<" cups"<<endl<<endl;

						cout<<"Maximum jugs using lemons: "<<sMaxJugsLemons<<endl;
						cout<<"Maximum jugs using sugar: "<<sMaxJugsSugarCups<<endl;
						cout<<"Maximum jugs using ice: "<<sMaxJugsIceCups<<endl<<endl;

					} // end of if

					// determine which stock item can create the minimum number of jugs and return the value
					if((sMaxJugsLemons <= sMaxJugsSugarCups) && (sMaxJugsLemons <= sMaxJugsIceCups))
					{
						sJugsToMake = sMaxJugsLemons;
						//cout<<"Lemons was selected as the maximum."<<endl;
					}
					else if((sMaxJugsSugarCups <= sMaxJugsLemons) && (sMaxJugsSugarCups <= sMaxJugsIceCups))
					{
						sJugsToMake = sMaxJugsSugarCups;
						//cout<<"Sugar was selected as the maximum."<<endl;
					}
					else if((sMaxJugsIceCups <= sMaxJugsLemons) && (sMaxJugsIceCups <= sMaxJugsSugarCups))
					{
						sJugsToMake = sMaxJugsIceCups;
						//cout<<"Ice was selected as the maximum."<<endl;
					}
					else
					{
						cout<<"An error occurred in trying to calculate the minimum stock item."<<endl;
					}
					// end of if/else statements

					if(sJugsToMake < 1)
					{
						cout<<"You dont have enough stock items to even make one jug."<<endl;
						system("pause");
					}
				}
				else if(cInput == '2')
				{
					do
					{
						cout<<"Make how many jugs of lemonade? (0 to cancel): ";
						cin>>cStringEntered;
						fflush(stdin); // clear the buffer

						blValidation = CheckEntryFormat(cStringEntered);

						sJugsToMake = atoi(cStringEntered);

						if((sJugsToMake < 0) || (sJugsToMake > 100))
						{
							cout<<"You must choose to make between 0 and 100 jugs."<<endl;
							blValidation = false;

						} // end of if

						if(blValidation == false)
							system("pause");

					} while(blValidation = false);

				} // end of if/else statements

				if(sJugsToMake > 0)
				{
					// call function that makes the specified amount of lemonade jugs
					ProcessLemonadeJugs(sJugsToMake);

				} // end of if for fJugsToMake

				else // if the player chose to cencel
				{
					system("cls");
				} // end of else
			break;

			case '3': // do nothing
				break;

			default: 
				cout<<"That was not a valid response."<<endl;
				PauseAndClear();

		} // end of switch statement

	} while(cInput != '3');

} // end of make Lemonade function

// function that creates the jugs of lemonade once the player has decided how much
void ProcessLemonadeJugs(unsigned short sJugsToMake)
{
	// declare local varaibles for ingredients needed
	float fNeededLemons, fNeededSugarCups, fNeededIceCups, fNeededSugarBags, fNeededIceBags;

	// calculate how many of each of the three ingredients will be required
	fNeededLemons = sJugsToMake * recipe.lemon;
	fNeededSugarCups = sJugsToMake * recipe.sugar;
	fNeededIceCups = (float)(sJugsToMake * recipe.ice);

	// calculate how many bags will be needed based on the number of cups per bag
	fNeededSugarBags = fNeededSugarCups / SUGAR_PER_BAG;
	fNeededIceBags = fNeededIceCups / ICE_CUPS_PER_BAG;

	do
	{
		// prompt purchase ifo to user
		cout<<"\nYou have chosen to fill "<<sJugsToMake<<" jugs of lemonade.\n"
			<<"Enough to sell "<<sJugsToMake*CUPS_PER_JUG<<" cups.\n\n"
			<<"This will require the following ingredients\n"
			<<fNeededLemons<<" Lemons\n"
			<<fNeededSugarBags<<" bags of sugar ("<<fNeededSugarCups<<" cups)\n"
			<<fNeededIceBags<<" bags of ice ("<<fNeededIceCups<<" cups)\n"
			<<endl
			<<"Do you wish to fill this many jugs? (1) Yes (2) No: ";
		cInput = getchar();
		fflush(stdin);

		if(cInput == '1')
		{
			// if there is enough of every stock item
			if((stock.lemons >= fNeededLemons) &&
			   (stock.sugarBags >= fNeededSugarBags) &&
			   (stock.iceBags >= fNeededIceBags))
			{		
				/* Decrease the number of items in stock
				   needed to make lemonade jugs */
				
				// decrease lemons needed from stock			
				stock.lemons -= fNeededLemons;
				
				// decrease sugar needed from stock			
				stock.sugarBags -= fNeededSugarBags;
				stock.sugarCups -= (unsigned short)(fNeededSugarBags * SUGAR_PER_BAG);
				
				// decrease ice needed from stock			
				stock.iceBags -= fNeededIceBags;
				stock.iceCups -= fNeededIceBags * ICE_CUPS_PER_BAG;
				stock.iceCubes -= (unsigned short)(fNeededIceBags * ICE_CUPS_PER_BAG * ICE_CUBES_PER_CUP);

				// increase the number of jugs made by what was requested
				stock.lemonadeJugs += sJugsToMake;
				stock.lemonadeCups += (unsigned short) (sJugsToMake * CUPS_PER_JUG);
				
				// confirm to user that the jugs were filled
				cout<<sJugsToMake<<" Jugs of lemonade has been filled and is now in stock\n";
				PauseAndClear();
				
			} // end if for checking the stock
			else
			{
				// check if player has enough of each item in stock to make the jugs
				if(stock.lemons < fNeededLemons) // if the player does not have enough lemons		
					cout<<"You do not have enough have enough lemons."<<endl;
				//end of if
				if(stock.sugarBags < fNeededSugarBags) // if the player does not have enough sugar
					cout<<"You do not have enough have enough bags of sugar."<<endl;
				//end of if
				if(stock.iceBags < fNeededIceBags) // if the player does not have enough ice
					cout<<"You do not have enough have enough bags of ice."<<endl;			
				// end of inner if conditions

				PauseAndClear();

			} // end of else for checking the stock

		} // end of if for cInput
		else
		{
			system("cls");
		} // end of else for cInput

	} while((cInput != '1') && (cInput != '2'));
	// end of do/while loop

} // end of ProcessLemonJugs function

// function for getting the random stock loss of lemons sugar and ice when lemonade is made
float GetRandomStockLoss(ITEM_SELECTED itemType)
{
	// declare local variables
	unsigned short sPercentLossed; // the random percentage of stock lossed
	//float fLemonsLossed, fSugarBagsLossed, sIceBagsLossed;

	switch(itemType)
	{
		case Lemons:
			sPercentLossed = rand()%PERCENT_LEMONS_MAX;
			return (stock.lemons / 100) * sPercentLossed;
		break;

		case Sugar:
			sPercentLossed = rand()%PERCENT_SUGAR_MAX;
			return (stock.sugarBags / 100) * sPercentLossed;
		break;

		case Ice:
			sPercentLossed = rand()%PERCENT_ICE_MAX;
			return (stock.iceBags / 100) * sPercentLossed;
		break;

		default:
			cout<<"Error: Invalid Stock item type found when getting random stock loss.\n"
				<<"Value found = "<<itemType<<endl;
			system("pause");
			return 0;
	} // end of switch statement

} // end of function

/********************
 * Currently unused *
 ********************/
// function that shows all stock purchases and the information about them
void ShowStockList()
{
	if(DebugMode == true)		
			cout<<"Stock Purchases: "<<data.stockPurchases<<endl;
	// end of if

	// display header of database table
	cout<<"No. Item Type Quantity Purchased Days in Expires in\n"
		<<"--- --------- -------- --------- ------- ----------"<<endl;
	
	for(int i=0; i<data.stockPurchases; i++) // iterate through the stockDays array
	{
		// display the array contents of the StockList structure
		cout<<i+1<<".   "<<stockDays[i].itemName<<"\t "<<stockDays[i].quantity<<'\t'
			<<"Day "<<stockDays[i].dayBought<<"  \t    "<<stockDays[i].daysIn<<"\t     "
			<<stockDays[i].daysLeft<<endl;

	} // end of for loop

} // end of ShowStockList function
