/* Customer.cpp
 * Programmer: Paul King
 * Last Updated: 4/11/2010
 * Description: This is the source file holding the functions that are used whenever the store is
 *				open and the sales process takes places. Everything starts within the ProcessDay
 *				function.
 *				Function prototypes, external variables and objects and other header files that
 *				this source file uses can be found in the "Customer.h" header file.
*/

// include header
#include "customer.h"

// Function that is called constantly from MaingameLoop.cpp while the game menu is on screen
void ProcessDay()
{
	// declare local variables
	unsigned short sTicksPassed = 0, // seconds  passed since the store opened
				   sTicksLeft = data.dayLength, // number of seconds left in the day,				   
				   sTicksFound = 4; // the amount of seconds that was found on the clock
									// after every 3 seconds
	float fSecondsPerTick = 1; // the number of seconds per each clock tick in the game
	
	bool blTurbo = false, // determines if the game is running in turbo speed
		 blSkipMode = false; // determines if the store sale process will be skipped right through

	unsigned short sPriceSatisfaction;
	char cEntered;	

	clock_t waitPeriod; // create an object for the wait period between each tick in the game

	// make the random numbers new ones each time the day begins 
	srand((int)time(NULL));

	// initialise daily values to 0
	financeDaily.customers = 0;
	financeDaily.cupsSold = 0;
	financeDaily.cupsLost = 0;
	financeDaily.cupsDestroyed = 0;
	financeDaily.recipeGood = 0;
	financeDaily.recipeBad = 0;
	financeDaily.priceGood = 0;
	financeDaily.priceTooHigh = 0;
	financeDaily.iceMelted = 0;
	financeDaily.moneyMade = 0.0f;	
	financeDaily.jugsSold = 0.0f;
	financeDaily.jugsLeftover = 0.0f;
	financeDaily.moneyMade = 0;
	financeDaily.totalProfit = 0;

	// calculate how frequently a customer will appear
	switch(weather.tempRange)
	{
		case Cold: customer.delay = 5;
			break;
		case Warm: customer.delay = 4;
			break;
		case Hot: customer.delay = 3;
			break;
		case Very_Hot: customer.delay = 5;
			break;
	} // end of swtich statement

	// if it is raining add another 2 seconds between each customer
	if(weather.rain == true)
		customer.delay += 2;
	// end of if
	
	ShowSalesInfo(sTicksPassed, sTicksLeft); // call function that displays sales information

	// as long as the timer is running and there is lemonade in stock
	while((sTicksPassed < data.dayLength) && (stock.lemonadeCups > 0))
	{
		waitPeriod = (clock_t) (clock() + fSecondsPerTick * CLOCKS_PER_SEC);

		if(blSkipMode == true)
		{
			if((sTicksLeft >= 4) && (sTicksPassed <= 116))
			{
				sTicksLeft -= customer.delay;
				sTicksPassed += customer.delay;
			}
			else
			{
				sTicksLeft = 0;
				sTicksPassed = 120;
			}
			// end of if/else

			// function that determines if the customer wants a cup
			sPriceSatisfaction = ProcessNewCustomer();
			// function that determines how many additional cups the customer wants to buy
			ProcessCupsBought(sPriceSatisfaction);
			// function that determines if anything in stock will be destroyed by an incident event
			GetIncidentChance();

			if(DebugMode == true)
				ShowSalesInfo(sTicksPassed, sTicksLeft);
			// end of if

		} // end of if
		
		else
		{			
			// repeat while the clock value is less than the value of the wait period
			while(clock() < waitPeriod)
			{ 
				if(_kbhit()) // if a key on the keyboard is pressed
				{	
					if(_getch() == '\n') // clear buffer if the enter key is pressed
						cin.ignore();
					do
					{				
						// get option from user
						cout<<"\n(1) Normal Speed, (2) Fast Speed, (3) Ultra Speed\n"
							<<"(s) Skip to end of day, (c) cancel\n"
							<<"Enter your option: ";
						cEntered = getchar();
						fflush(stdin);
						
						// change game speed or skip accordingly
						switch(cEntered)
						{								
							case '1': fSecondsPerTick = 1.00f;
								break;
							case '2': fSecondsPerTick = 0.10f;
								break;
							case '3': fSecondsPerTick = 0.01f;
								break;
							case 's': case 'S':	blSkipMode = true;
								break;
							case 'c': // do nothing
								break;
							default: cout<<"That answer was invalid. Try again."<<endl;						
							
						} // end of switch statement

					} while((cEntered != '1') && (cEntered != '2') && (cEntered != '3') &&
						   (cEntered != 's') && (cEntered != 'S') && (cEntered != 'c')&& (cEntered != 'C'));
			
				} // end of if for kbhit
			} // end of while loop for clock

			sTicksPassed++; // increment the number of seconds passed in the day			
			sTicksLeft--; // decrement the number of seconds left in the day
			
			// if the number of lemonade jugs goes into negatvie figures put it back on 0
			if(stock.lemonadeJugs < 0)
				stock.lemonadeJugs = 0;
			// end of if			
			
			// if the number of seconds that has passed is divisable by four
			// and there is atleast 1 cup in stock
			if(sTicksPassed % customer.delay == 0)
			{			
				sTicksFound = sTicksPassed; // set the seconds found to the seconds currently passed

				// add new text to customer string
				AddStoreMessage("A customer has arrived at the store.");

				financeDaily.customers++;

			} // end of if for 4 seconds			

			// if 1 second has passed since a customer arrived
			if(sTicksPassed == sTicksFound + 1)
			{
				// call function the processes the customer's action
				sPriceSatisfaction = ProcessNewCustomer();
			} 
			// end of if for 1 second after the customer arrived			

			// if 2 seconds has passed since the custome arrived and he/she bought a cup
			if((sTicksPassed == sTicksFound + 2) && (sPriceSatisfaction > 0)
				&& (stock.lemonadeCups > 0))
			{
				// call function that determines how many more cups will be bought
				ProcessCupsBought(sPriceSatisfaction);

				// insert a dashed line for sepration for easier reading
				AddStoreMessage("--------------------------------------------------------------------");
			}
			// end of if for 2 seconds after the customer arrived

			if(sTicksPassed % 5 == 0)
			{
				// determines if anything in stock will be destroyed by an incident event				
				GetEvent();
			}
			// end of if for every 5 seconds

		} // end of else for blSkipMode

		// if lemonade jugs go into a negative figure put in back on 0
		if(stock.lemonadeJugs < 0)
			stock.lemonadeJugs = 0;
		// end of if

		ShowSalesInfo(sTicksPassed, sTicksLeft); // call function that displays sales information
	
	} // end of while loop for store open process

	if(sTicksPassed >= data.dayLength)
		cout<<"\nThe day has come to an end and the store is now closed."<<endl;
	else if(stock.lemonadeCups == 0)
		cout<<"\nYou have run out of lemonade in stock and can not sell anymore for today."<<endl;
	//end of if/else
	system("pause");

	// increase the length of the day	
	data.dayLength += data.dayLengthIncrease;

	EndDay(sTicksLeft, data.dayLength); // call function to perform the ending processes

	// reset the number of seconds passed and left in the day and the speed modes
	sTicksPassed = 0;
	sTicksLeft = data.dayLength;

	// increase amount to increase the day by
	data.dayLengthIncrease += 10;

} // end of UpdateTimer function

// Function that shows the sales information
void ShowSalesInfo(unsigned short sTicksPassed, unsigned short sTicksLeft)
{		
	system("cls");

	// if debug mode is on show daily report information
	if(DebugMode == true)
	{
		cout<<"Timer: "<<sTicksPassed<<'\t'<<"Message lines used: "<<sLinesUsed<<" of "<<MAX_LINES<<endl;
		cout<<"Customer Frequency: "<<customer.delay<<" seconds"<<endl<<endl;

		cout<<"Current Stock\n"
			<<"-------------\n"
			<<"Lemons: "<<stock.lemons<<endl
			<<"Sugar Bags: "<<stock.sugarBags<<endl
			<<"Ice Bags: "<<stock.iceBags<<endl<<endl;

		cout<<"Sales Made: "<<financeDaily.cupsSold<<" ("<<financeDaily.cupsSold/CUPS_PER_JUG<<" Jugs)"<<endl;
		printf("Sale Price: $%.2f\n\n",prices.lemonadePrice);
			
		cout<<"Satisfied With Recipe: "<<financeDaily.recipeGood<<endl
			<<"Unsatisfied With Recipe: "<<financeDaily.recipeBad<<endl
			<<"Happy With Price: "<<financeDaily.priceGood<<endl
			<<"Unhappy With Price: "<<financeDaily.priceTooHigh<<endl
			<<endl;
		printf("Money made so far: $%.2f\n\n",financeDaily.moneyMade);
		
		cout<<"Change of Ants: "<<PERCENT_CHANCE_ANTS<<'%'<<endl;
		/*
		if(weather.rain == true)
			cout<<"Chance of Rodents: "<<sPercentChanceRodents<<'%'<<endl;
		// end of if
		*/
		cout<<"Chance of Kids: "<<PERCENT_CHANCE_KID<<'%'<<endl<<endl;		

		cout<<"Lemons destroyed by rodents: "<<financeDaily.lemonsInfested<<endl
			<<"Sugar destroyed by rodents: "<<financeDaily.lemonsOff<<endl
			<<"Cups knocked over by kids: "<<financeDaily.cupsDestroyed<<endl
			<<endl;

	} // end of if

	cout<<"Day"<<data.dayNum<<"\tTime Left: "<<sTicksLeft<<"\t "
		<<"Press any key to pause or access options."<<endl<<endl;

	ShowWeather(); // call function that displays today's weather

	// display the stock, money and customer information
	printf("Lemonade In Stock: Jugs: %.2f ", stock.lemonadeJugs);
	cout<<"Cups: "<<stock.lemonadeCups<<endl;
	printf("Current Money: $%.2f\n\n", data.currentMoney);	
	cout<<"Store Info\n"
		<<"----------"<<endl;

	// display all the store text
	for(int i=0; i<MAX_LINES; i++)
	{
		if(DebugMode == true)
			cout<<i+1<<": ";
		// end of if
		cout<<strStoreInfo[i]<<endl; 
	} // end of for loop

} // End of ShowSalesInfo function

// function the adds text to store message as well as scroll the lines
void AddStoreMessage(char* strMessage)
{
	// if the lines currently used is less than the maximum add information to nex line
	if(sLinesUsed < MAX_LINES)
	{
		strcpy_s(strStoreInfo[sLinesUsed], 100, strMessage);
		sLinesUsed++;
	}
	else
	{	
		/* if the maximum number of lines have been uses, remove the first line, push all
		   the following text back one line and add message onto the last line */
		for(int i=0; i<MAX_LINES; i++)
			strcpy_s(strStoreInfo[i], 100, strStoreInfo[i+1]);			
		// end of for loop

		strcpy_s(strStoreInfo[sLinesUsed-1], 100, strMessage);
	} 
	// end of if/else

} // end of Add Store Text function

// Function gets a random event while the store is open
void GetEvent()
{
	// local variables
	unsigned short sEventRoll, sDestroyQuantity;
	STORE_EVENT sRandomEvent;

	sEventRoll = rand()%100; // randomly pick a number between 0 and 100

	if(sEventRoll < PERCENT_CHANCE_EVENT) // if the number generated is less than the percentage
	{
		// randomly pick a number between 0 and 5 for the event
		sRandomEvent = (STORE_EVENT)(rand()%5);

		if(DebugMode == true)
			cout<<"Random event number: "<<sRandomEvent<<endl;
		// end of if

		switch(sRandomEvent)
		{
			case Knocked_Over:
				// get the random amount of lemonade to knock over
				sDestroyQuantity = rand()%(KID_KNOCKED_MAX-KID_KNOCKED_MIN) + KID_KNOCKED_MIN;

				/* if the random amount of cups to destroy is greate than the amount that is in stock
				   destroy whatever is left */
				if(sDestroyQuantity > stock.lemonadeCups)
					sDestroyQuantity = stock.lemonadeCups;
				// end of if

				if(sDestroyQuantity > 0)
				{					
					// remove the select amount lemonade form stock
					stock.lemonadeCups -= sDestroyQuantity;
					stock.lemonadeJugs -= sDestroyQuantity * CUPS_PER_JUG;

					// update the finances
					financeDaily.cupsDestroyed += sDestroyQuantity;
					financeTotal.cupsDestroyed += sDestroyQuantity;

					// show the message to the user
					sprintf_s(strCurrentMessage, 100, "A larrikin knocked over %i cups of lemonade.",sDestroyQuantity);
					AddStoreMessage(strCurrentMessage);
				} // end of if
			break;

			case Rodent:
				sDestroyQuantity = 1;

				if(sDestroyQuantity > stock.sugarBags)
					sDestroyQuantity = (unsigned short)stock.sugarBags;
				// end of if
				
				if(sDestroyQuantity > 0)
				{
					// remove the select amount of sugar from stock
					stock.sugarBags -= sDestroyQuantity;
					stock.sugarCups -= sDestroyQuantity * SUGAR_PER_BAG;

					// update the finances
					financeDaily.sugarInfestRodents += sDestroyQuantity;
					financeTotal.sugarInfestRodents += sDestroyQuantity;

					// show the message to the user
					sprintf_s(strCurrentMessage, 100, "A rodent soiled a bag of sugar", sDestroyQuantity);
					AddStoreMessage(strCurrentMessage);
				} // end of if
			break;

			case Ball:
				sDestroyQuantity = rand()%(BALL_KNOCKED_MAX-BALL_KNOCKED_MIN) + BALL_KNOCKED_MIN;

				if(sDestroyQuantity > stock.lemonadeCups)
					sDestroyQuantity = stock.lemonadeCups;
				// end of if

				if(sDestroyQuantity > 0)
				{
					// remove the select amount lemonade form stock
					stock.lemonadeCups -= sDestroyQuantity;
					stock.lemonadeJugs -= sDestroyQuantity * CUPS_PER_JUG;

					// update the finance
					financeDaily.cupsDestroyed += sDestroyQuantity;
					financeTotal.cupsDestroyed += sDestroyQuantity;

					// show the message to the user
					sprintf_s(strCurrentMessage, 100, "A ball flew over and knocked over %i cups of lemonade",
							  sDestroyQuantity);
					AddStoreMessage(strCurrentMessage);
				} // end of if
			break;

			case Steal:
				sDestroyQuantity = rand()%(STOLEN_CUPS_MAX-STOLEN_CUPS_MIN) + STOLEN_CUPS_MIN;

				if(sDestroyQuantity > stock.lemonadeCups)
					sDestroyQuantity = stock.lemonadeCups;
				// end of if

				if(sDestroyQuantity > 0)
				{
					// remove the select amount lemonade form stock
					stock.lemonadeCups -= sDestroyQuantity;
					stock.lemonadeJugs -= sDestroyQuantity * CUPS_PER_JUG;

					// show the message to the user
					sprintf_s(strCurrentMessage, 100, "A student ran off with %i cups of lemonade without paying",
							  sDestroyQuantity);
					AddStoreMessage(strCurrentMessage);
				} // end o if
			break;

			case Homeless:
				sDestroyQuantity = rand()%(HOMELESS_GIVEN_MAX-HOMELESS_GIVEN_MIN) + HOMELESS_GIVEN_MIN;

				if(sDestroyQuantity > stock.lemonadeCups)
					sDestroyQuantity = stock.lemonadeCups;
				// end of if

				if(sDestroyQuantity > 0)
				{
					// remove the select amount lemonade form stock
					stock.lemonadeCups -= sDestroyQuantity;
					stock.lemonadeJugs -= sDestroyQuantity * CUPS_PER_JUG;

					// show the message to the user
					if(sDestroyQuantity > 1)
						sprintf_s(strCurrentMessage, 100, "%i free cups of lemonade were given out to some homeless people",
								  sDestroyQuantity);
					else
						sprintf_s(strCurrentMessage, 100, "A free cup of lemonade was given to a homeless person.");
					//end of if

					AddStoreMessage(strCurrentMessage);
				} // end of if
			break;			

			default: 
				cout<<"Error: a random value of "<<sRandomEvent<<" was found when selecting a random event."<<endl;
				system("pause");

		} // end of switch statement
	} // end of if

} // end of getEvent function

// Function sed to decide how much lemonade is bought. Called whenever a customer arrives
unsigned short ProcessNewCustomer()
{
	// declare local variable
	unsigned short sPriceSatisfaction;

	sPriceSatisfaction = GetPriceDecision();

	if(DebugMode == true)
	{
		cout<<"Price satisfaction value: "<<sPriceSatisfaction<<endl;
		//system("pause");
	} // end of if

	if(sPriceSatisfaction > 0) // if the customer is happy with the price
	{	
		// update the number of of cups and jugs in stock and the total cups sold
		stock.lemonadeCups -= 1;
		stock.lemonadeJugs -= (float)1 / CUPS_PER_JUG;
		financeTotal.cupsSold ++;

		// update the money total money earned and the total player money
		financeTotal.moneyMade += prices.lemonadePrice;
		data.currentMoney += prices.lemonadePrice;

		// update the daily and total money earned
		financeDaily.moneyMade += (prices.lemonadePrice);
		financeTotal.moneyMade += (prices.lemonadePrice);		
		
		sprintf_s(strCurrentMessage, 100, "The customer buys a cup of lemonade (+$%.2f)", 
										  prices.lemonadePrice);	
		AddStoreMessage(strCurrentMessage);

		// increase the number of cups sold and the number of customers who accepted the price
		financeDaily.cupsSold++;
		financeDaily.priceGood++;
		financeTotal.cupsSold++;
		financeTotal.priceGood++;

	} // end of if for prices.lemonadePrice
	else
	{
		AddStoreMessage("The customer decided that the price of your lemonade was too high.");

		// increment the number of people who thought the price was too high
		financeDaily.priceTooHigh++;
		financeTotal.priceTooHigh++;

	} // end of else

	return sPriceSatisfaction;

} // end of ProcessNewcustomer function

// Function that determines whether or not the customer found the price reasonable
unsigned short GetPriceDecision()
{
	float fMaxPriceCustomer; // the maximum amount that the customer will pay

	// randomly pick a number between 0 and 99 for the customer cheapenss level
	customer.cheapness = rand()%100; 
	// determine the maximum the customer will pay based on the base max price
	fMaxPriceCustomer = (float)(BASE_MAX_PRICE + customer.cheapness * 0.01);

	// if the price is half or less of the maximum price the customer will pay
	if((prices.lemonadePrice*2) <= fMaxPriceCustomer)
		return 2; // the customer will buy double the amount
	// if the price is not half but less than or equal to the maximum price
	else if(prices.lemonadePrice <= fMaxPriceCustomer)
		return 1; // the customer will buy the standard amount
	else // if the price is greater than the maximum they will pay
		return 0; // the customer will not buy a cup
	// end of if/else conditions

} // end of GetPriceDecision function

// Function that determins how many extra cups the customer is buying
void ProcessCupsBought(unsigned int sPriceSatisfaction)
{
	// declare local variables
	short sCupsBought; // the number of cups the customer chose to buy		  
	float fMoneyReceived, // the made made from 1 customer
		  fJugsRemoved; // the amount of jugs removed during purchase
	TASTE_RESULT eFoundTaste; // determins if the customer likes the recipe

	char strRecipeStatus[6]; // string that hold the recip value if the customer does not like it

	// call the function that tests the recipe and return result to variable
	eFoundTaste = GetTasteApproval();

	switch(eFoundTaste)
	{	
		case Good: // if the customer likes the recipe do nothing
			break;
		case Too_Sour: strcpy_s(strRecipeStatus, 6, "sour");
			break;
		case Too_Sweet: strcpy_s(strRecipeStatus, 6, "sweet");
			break;
		case Too_Warm: strcpy_s(strRecipeStatus, 6, "warm");			
			break;		
		case Too_Cold: strcpy_s(strRecipeStatus, 6, "cold");		
			break;

	} // end of switch

	if(eFoundTaste == Good)
	{
		if(sPriceSatisfaction > 1) // if the price is less than $1.10
			// generate a random number between 2 and 10 for the number of cups bought
			sCupsBought = (rand()%(CUPS_BOUGHT_MAX+CUPS_BOUGHT_MIN)+CUPS_BOUGHT_MIN) * 2;
		else		
			// generate a random number between 1 and 5 for the number of cups bought
			sCupsBought = rand()%(CUPS_BOUGHT_MAX+CUPS_BOUGHT_MIN)+CUPS_BOUGHT_MIN;	
		// end of if/else

		// if the amount chosen to purchase is more than the stock left
		if(sCupsBought > stock.lemonadeCups)
		{
			sCupsBought = stock.lemonadeCups; // the customer buys whatever is left

			//stock.lemonadeJugs = 0; // set the jug stock to 0 to prevent negative figure

		} // end of if
	
		// decrease the amount of cups in stock by the amount bought
		stock.lemonadeCups -= sCupsBought;
		// decrease the appropriate amount of jugs in stock
		fJugsRemoved = (float)sCupsBought / CUPS_PER_JUG;
		stock.lemonadeJugs -= fJugsRemoved;
		// increase the total number of cups sold
		financeTotal.cupsSold += sCupsBought;

		// the money received = the price multiplied by the amount bought
		fMoneyReceived = prices.lemonadePrice * sCupsBought;
		// increase the player's money by the amount the customer paid
		data.currentMoney += fMoneyReceived;
		// increase the total money made by the money received
		financeDaily.moneyMade += fMoneyReceived;
		financeTotal.moneyMade += fMoneyReceived;
		// increase the cup sales by the amount bought
		financeDaily.cupsSold += sCupsBought;
		// increment the number of people who liked the recipe
		financeDaily.recipeGood++;
		financeTotal.recipeGood++;

		if(sPriceSatisfaction > 0)
		{					
			sprintf_s(strCurrentMessage, 100,
					 "The customer decides to buy %i more cups due to low price. (+$%.2f)",
					  sCupsBought, fMoneyReceived);
		}
		else
		{					
			sprintf_s(strCurrentMessage, 100,
					  "The customer decides to buy %i more cups (+$%.2f)",
					  sCupsBought, fMoneyReceived);
					
		} // end of if/else

	} // end of if for eFoundTaste
	else
	{			
		sprintf_s(strCurrentMessage, 100,
				  "The customer found your recipe too %s and chose not to buy anymore",
				  strRecipeStatus);

		// increment the number of people who did not like the recipe
		financeDaily.recipeBad++;
		financeTotal.recipeBad++;

	} // end of else for eFoundTaste

	AddStoreMessage(strCurrentMessage);

} // end of ProcessCupsBought function

// Function that determins that determines whether or not the cstome likes that recipe
TASTE_RESULT GetTasteApproval()
{
	// local variables
	unsigned short sTemperatureScore, // the percentage of chance you have of selling the lemonade				   
				   sSuccessMark; // what the random number must be greater or equal to make a success

	// enumerator variables for recipe statuses
	TASTE_PREFERENCE eTastePreference = (TASTE_PREFERENCE) (rand()%5);	
	// call function that determines sweet and sour level
	RECIPE_SWEETNESS eRecipeTaste = GetRecipeSweetness();
	// call function to determine the temperature of the lemonade and return the odds of it selling
	RECIPE_TEMPERATURE eRecipeTempureture = GetRecipeTemperature();

	switch(eRecipeTempureture)
	{
		case Very_Cold: case Very_Warm:
			sTemperatureScore = 0;
			break;
		case Somewhat_Cold: case Somewhat_Warm:
			sTemperatureScore = 50;
			break;
		case Slightly_Cold: case Slightly_Warm:
			sTemperatureScore = 75;
			break;
		case Ideal:
			sTemperatureScore = 100;
			break;
		default: 
			cout<<"An error has occured with the ice condition in the recipe.\n"
				<<"Value Found = "<<eRecipeTempureture<<endl;
			system("pause");
			sTemperatureScore = 100;

	} // end of switch statement

	// generate random value for how likely the customer will approve of the temperature
	customer.iceTolerance = rand()%100;	
	sSuccessMark = 100 - sTemperatureScore; // subtract the temperature score from 100

	if(DebugMode == true)
		cout<<"Taste Preference: "<<eTastePreference<<endl;
	// end of if

	switch(eTastePreference)
	{
		case Sour_Only:
			switch(eRecipeTaste)
			{
				case Very_Sour: 
					return Too_Sour;
				case Sour: case Slightly_Sour:
					break;
				case Average: case Slightly_Sweet: case Sweet: case Very_Sweet:
					return Too_Sweet;
			} // end of innwer switch 1
		break;

		case Sour_And_Standard:
			switch(eRecipeTaste)
			{
				case Very_Sour: 
					return Too_Sour;
				case Sour: case Slightly_Sour: case Average:
					break;
				case Slightly_Sweet: case Sweet: case Very_Sweet:
					return Too_Sweet;
			} // end of inner switch 		
		break;

		case Standard:
			switch(eRecipeTaste)
			{
				case Very_Sour: case Sour:
					return Too_Sour;
				case Slightly_Sour: case Average: case Slightly_Sweet:
					break;
				case Sweet: case Very_Sweet:
					return Too_Sweet;
			} // end of inner switch 		
		break;

		case Sweet_And_Standard:
			switch(eRecipeTaste)
			{
				case Very_Sour: case Sour: case Slightly_Sour:
					return Too_Sour;
				case Average: case Slightly_Sweet: case Sweet:
					break;
				case Very_Sweet:
					return Too_Sweet;
			} // end of inner switch 		
		break;

		case Sweet_Only:
			switch(eRecipeTaste)
			{
				case Very_Sour: case Sour: case Slightly_Sour: case Average:
					return Too_Sour;
				case Slightly_Sweet: case Sweet:
					break;
				case Very_Sweet:
					return Too_Sweet;
			} // end of inner switch		
		break;

		default: cout<<"An error occured with the condition of the recipe."<<endl;
				 system("pause");
			// do nothing

	} // end of outter switch statement

	if(customer.iceTolerance >= sSuccessMark)
		return Good;
	else if((eRecipeTempureture == Very_Cold) || (eRecipeTempureture == Somewhat_Cold) ||
			(eRecipeTempureture == Slightly_Cold))
		return Too_Cold;
	else if((eRecipeTempureture == Very_Warm) || (eRecipeTempureture == Somewhat_Warm) ||
			(eRecipeTempureture == Slightly_Warm))
		return Too_Warm;
	else
		cout<<"An error occured between the temperature customer sastifaction and\n"
			  "the condition of the lemonade temperature."<<endl;
	// end of if/else statements

	system("pause");
	return Good;

} // end of GetTasteDecision function

// Function that determines what the condition of the recipe is and returns it
RECIPE_SWEETNESS GetRecipeSweetness()
{	
	/************************************
	 * Recipe Sweet And Sour Conditions *
	 ************************************/
	/* If the number of lemons is at least 3 times the number of sugar or
	   the amount of sugar is equal to the minimum allowed */
	if((recipe.lemon > (recipe.sugar * 3)) || (recipe.sugar < 1))
		return Very_Sour;	
	
	// If the number of lemons 2.5 of more times the number of sugar cups
	else if(recipe.lemon >= (recipe.sugar * 2.5))
		return Sour;

	// if the number of lemons are more than twice the number of sugar cups
	else if(recipe.lemon > (recipe.sugar * 2))
		return Slightly_Sour;

	// if the number of lemons are twice the number of sugar cups
	else if(recipe.lemon == (recipe.sugar * 2))
		return Average;

	// if the lemons are atleast 1.5 times the amount sugar
	else if(recipe.lemon >= (recipe.sugar * 1.5))
		return Slightly_Sweet;

	// if there is more lemons than sugar but less than 1.5 times as many
	else if(recipe.lemon > recipe.sugar)
		return Sweet;

	// if there is less lemons in the recipe than sugar or less than 1 lemon in the recipe
	else if((recipe.lemon < recipe.sugar) || (recipe.lemon <1))
		return Very_Sweet;

	else
		cout<<"An error occured with the lemon and sugar value in the recipe."<<endl;
	// end of if/else conditions

	system("pause");
	return Average;	

} // end of GetRecipeSweetness function

// Function that determines how hot or cold the lemonade is
RECIPE_TEMPERATURE GetRecipeTemperature()
{
	/*********************************
	 * Recipe Temperature Conditions *
	 *********************************/
	switch(weather.tempRange)
	{
		case Cold: // temperature is less than 20		
			if(recipe.ice > 3)
				return Very_Cold;
			else if(recipe.ice == 3)
				return Somewhat_Cold;
			else if((recipe.ice == 2) || (recipe.ice == 1))
				return Ideal;
			else if(recipe.ice < 1)
				return Somewhat_Warm;
			else
			{
				cout<<"Error with ice value in the recipe."<<endl;
				system("pause");
			}
		break;

		case Warm: // temperature is between 20 and 29
			if(recipe.ice > 4)
				return Very_Cold;
			else if(recipe.ice == 4)
				return Somewhat_Cold;
			else if(recipe.ice == 3)
				return Ideal;	
			else if(recipe.ice == 2)
				return Slightly_Warm;
			else if(recipe.ice == 1)
				return Somewhat_Warm;
			else if(recipe.ice < 1)
				return Very_Warm;	
			else
			{
				cout<<"Error with ice value in the recipe."<<endl;
				system("pause");
			}
			// end of if/else conditions
		break;

		case Hot: // temperature is between 30 and 39
			if(recipe.ice > 5)
				return Very_Cold;
			else if(recipe.ice == 5)
				return Somewhat_Cold;
			else if(recipe.ice == 4)
				return Ideal;	
			else if(recipe.ice == 3)
				return Slightly_Warm;
			else if(recipe.ice == 2)
				return Somewhat_Warm;
			else if(recipe.ice < 2)
				return Very_Warm;	
			else
			{
				cout<<"Error with ice value in the recipe."<<endl;
				system("pause");
			}
			// end of if/else conditions
		break;

		case Very_Hot: // temperature is 40 or higher
			if(recipe.ice > 6)
				return Very_Cold;
			else if(recipe.ice == 6)
				return Somewhat_Cold;
			else if(recipe.ice == 5)
				return Ideal;	
			else if(recipe.ice == 4)
				return Slightly_Warm;
			else if(recipe.ice == 3)
				return Somewhat_Warm;
			else if(recipe.ice < 2)
				return Very_Warm;	
			else
			{
				cout<<"Error with ice value in the recipe."<<endl;
				system("pause");
			}
		break;

		default: cout<<"Error: An invalid weather temperature was detected\n"
						"       Value found = "<<weather.tempRange<<endl;
				 system("pause");				 

	} // end of switch statement

	return Ideal; // return the default value

} // end of GetRecipeTemperature function

// Function that executes the end of day processes
void EndDay(unsigned short ticksLeft, unsigned short timeLength)
{
	// clear the entire contents of the strStoreInfo string array
	for(int i=0; i<MAX_LINES; i++)
		strStoreInfo[i][0] = '\0';
	// end of for loop

	//UpdateStockTimers(); // call function to check stock time periods and update them

	// if the temperature is 30 degrees or more 15% of lemons go off
	if(weather.tempCel > 30)
	{		
		financeDaily.lemonsOff = (stock.lemons / 100) * 15;

		if(stock.lemons < financeDaily.lemonsOff)
			financeDaily.lemonsOff = stock.lemons;
		// end of if
		financeTotal.lemonsOff += financeDaily.lemonsOff;
		stock.lemons -= financeDaily.lemonsOff;
	} // end of if

	// randomly select an amount of sugar to be destroyed by ants
	financeDaily.sugarInfestAnts = rand()%(DESTROYED_SUGAR_MIN-DESTROYED_SUGAR_MAX) + DESTROYED_SUGAR_MIN;

	if(stock.sugarCups < financeDaily.sugarInfestAnts)
		financeDaily.sugarInfestAnts = (unsigned short)stock.sugarCups;
	// end of if

	// remove the require amount of sugar from stock and update the total amount removed
	financeTotal.sugarInfestAnts += financeDaily.sugarInfestAnts;
	stock.sugarCups -= financeDaily.sugarInfestAnts;
	stock.sugarBags -= financeDaily.sugarInfestAnts / SUGAR_PER_BAG;

	// set the amount of ice to melt
	financeDaily.iceMelted = (unsigned short)stock.iceCubes;
	// increase the total amount of ice melted
	financeTotal.iceMelted += financeDaily.iceMelted;

	// set all ice in the stock to 0
	stock.iceBags = 0;
	stock.iceCups = 0;
	stock.iceCubes = 0;

	if(stock.lemonadeJugs > 0)
	{
		// get the amount of lemonade not sold
		financeDaily.jugsLeftover = stock.lemonadeJugs;
		financeDaily.cupsLost = stock.lemonadeCups;
		// remove or remaining lemonade from stock
		stock.lemonadeCups = 0;
		stock.lemonadeJugs = 0;

	} // end of if
	
	ShowDailyReport(ticksLeft, timeLength); // call funtion to display a report on the current day

	data.dayNum++; // increase the current day by 1	
	sLinesUsed = 0; // reset the number of message line being used to 0
	financeDaily.moneySpent = 0; // reset the money spent for each day back to 0

} // end of EndDay function

// Function that displays the report of what happend in the day
void ShowDailyReport(unsigned short ticksLeft, unsigned short timeLength)
{
	// calculate finance figures
	financeDaily.jugsSold = (float)(financeDaily.cupsSold / CUPS_PER_JUG);
	financeDaily.totalProfit = financeDaily.moneyMade - financeDaily.moneySpent;

	system("cls");

	cout<<"Report of Day "<<data.dayNum<<endl
		<<"----------------\n"
		<<"Sales Made: "<<financeDaily.cupsSold<<" ("<<financeDaily.jugsSold<<" Jugs)"<<endl;
	printf("Sale Price: $%.2f\n\n",prices.lemonadePrice);

	if(ticksLeft > 0)
		cout<<"You ran out of stock with "<<ticksLeft<<" ticks left on the clock."<<endl<<endl;
	else
		cout<<financeDaily.cupsLost<<" cups of lemonade did not get sold and had to be thrown out("
		<<financeDaily.jugsLeftover<<" Jugs)"<<endl;
	// end of if

	cout<<financeDaily.recipeGood<<" customers were satisfied with your recipe.\n"
		<<financeDaily.recipeBad<<" customers were unsatisfied with your recipe.\n"
		<<financeDaily.priceGood<<" customers found your price reasonable\n"
		<<financeDaily.priceTooHigh<<" customers found the price too high.\n"
		<<endl
		<<financeDaily.cupsDestroyed<<" cups were lost from accidents."<<endl<<endl;
	if(weather.tempCel > 30)
		cout<<financeDaily.lemonsOff<<" went off in the heat."<<endl;
	// end of if
	cout<<financeDaily.sugarInfestRodents<<" bags of sugar were infested by a rodent.\n"
		<<financeDaily.sugarInfestAnts<<" cups of sugar were infested by ants.\n"
		<<financeDaily.iceMelted<<" ice cubes have melted ("<<financeDaily.iceMelted<<" bags).\n"		
		<<endl;
	printf("Money spent on stock today: $%.2f\n", financeDaily.moneySpent);
	printf("Money made from sales today: $%.2f\n", financeDaily.moneyMade);
	printf("Daily profit: $%.2f\n\n", financeDaily.totalProfit);

	cout<<"You can now have the store open for "<<timeLength<<" seconds."<<endl;

	PauseAndClear();

} // end of ShowDailyReport function

/*******************************
 * Functions Currently unnused *
 *******************************/
// Function that determines if items are destroyed and the quantity using random numbers
void GetIncidentChance()
{
	// local variables
	unsigned short sRandomChanceRodent, // random roll for the chance of a rodent infesting sugar
				   sRandomChanceAnts, // random roll for the chance of ants infesting sugar
				   sRandomChanceKid, // random roll for the chance of a kid causing a hazard
				   sPercentChanceRodent; // the odds of a rodent infesting the stock
	
	// if it is raining there is a 20% chance of a rodent, otherwise it is 5
	if(weather.rain == true)
		sPercentChanceRodent = 20;
	else
		sPercentChanceRodent = 5;
	// end of if

	// generate random numbers between 1 and 100 for variables
	if(weather.rain == false) // only generate a random number for ants if there is no rain
		sRandomChanceAnts = rand()%100;
	else
		sRandomChanceAnts = 0;
	// end of if/els

	sRandomChanceRodent = rand()%100;
	sRandomChanceKid = rand()%100;	

	/* if the random number falls into range of the chance percent call function to 
	   destroy the stock */
	if(stock.sugarCups > 0)
	{
		if(sRandomChanceAnts < PERCENT_CHANCE_ANTS)
			DestroyStock(stock.sugarCups, Sugar, "Ants");
		// end of inner if 1
		if(sRandomChanceRodent < sPercentChanceRodent)
			DestroyStock(stock.sugarCups, Sugar, "A Rodent");
		// end of inner if 2
	}
	// end outter if
	if(sRandomChanceKid < PERCENT_CHANCE_KID)
		DestroyStock(stock.lemonadeCups, Lemonade, "A Kid");
	// end of if

} // end of function

// Function that checks stock time periods and updates them or removes the stock
void UpdateStockTimers()
{
	for(int i=0; i<data.stockPurchases; i++)
	{
		// increment the amount of days each stock item has been in by 1
		stockDays[i].daysIn++;

		if((stockDays[i].itemType == Lemons) && (stockDays[i].daysIn >= TIME_LENGTH_LEMONS))
		{
			cout<<stockDays[i].quantity<<" lemons have gone off."<<endl;
			RemoveFromStock(Lemons, stockDays[i].quantity);
		}			
		else if((stockDays[i].itemType == Sugar) && (stockDays[i].daysIn >= TIME_LENGTH_SUGAR))
		{
			cout<<stockDays[i].quantity<<" bags of sugar have gone off."<<endl;
			RemoveFromStock(Sugar, stockDays[i].quantity);
		}			
		else if((stockDays[i].itemType == Ice) && (stockDays[i].daysIn >= TIME_LENGTH_ICE))
		{
			cout<<stockDays[i].quantity<<" bags of ice have melted."<<endl;
			RemoveFromStock(Ice, stockDays[i].quantity);
		}			
		// end of if/else conditions

	} // end of for loop

} // end of function
// Function that removes lemons and sugar from stock when infested by rodents
void DestroyStock(float stockAvailable, ITEM_SELECTED itemUsed, char* causer)
{
	// declare local variables	
	unsigned short sQuantityCups; // the quantity of lemonade cups
	float fQuantitySugar; // the quantity of sugar to be destroyed

	// if the items selected exist in stock
	if(stockAvailable > 0)
	{
		switch(itemUsed)
		{
			case Sugar:
				// get the random amount of sugar to destroy
				fQuantitySugar = (float)(rand()%(DESTROYED_SUGAR_MAX - DESTROYED_SUGAR_MIN))
					+ DESTROYED_SUGAR_MIN;

				// if the amount in stock is less than the ammount to destroy, destroy the whole stock
				if(stockAvailable < fQuantitySugar)
					fQuantitySugar = stockAvailable;
				// end of if

				// add the sugar amount that was destroyed to finances
				financeDaily.sugarInfestRodents += fQuantitySugar;
				financeTotal.sugarInfestRodents += fQuantitySugar;
				
				// remove the required number of lemons from stock
				stock.sugarCups -= fQuantitySugar;
				stock.sugarBags -= (fQuantitySugar / SUGAR_PER_BAG);

				// display how many lemons were destroyed
				if(fQuantitySugar == DESTROYED_SUGAR_MAX)
					sprintf_s(strCurrentMessage, 100, "%s infested 1 of your sugar bags.", 
							  causer, fQuantitySugar);					
				else					
					sprintf_s(strCurrentMessage, 100, "A %s infested %.0f cups of sugar.", 
							  causer, fQuantitySugar);
				// end of if/else statements

				// update the number of sugar cups infested today
				financeDaily.sugarInfestRodents += fQuantitySugar;
			break;

			case Lemonade:
				// get the random amount of lemonade cups to destroy
				sQuantityCups = ( (rand()%(DESTROYED_CUPS_MAX - DESTROYED_CUPS_MIN)) 
					+ DESTROYED_CUPS_MIN );

				// if the amount in stock is less than the ammount to destroy, destroy the whole stock
				if(sQuantityCups < sQuantityCups)
					sQuantityCups = (unsigned short)stockAvailable;
				// end of if

				// remove the required number of lemonade cups from stock
				stock.lemonadeCups -= (unsigned short)sQuantityCups;
				stock.lemonadeJugs -= (sQuantityCups / CUPS_PER_JUG);

				// diplay how much lemonade was destroyed
				if(sQuantityCups == DESTROYED_CUPS_MAX)
					sprintf_s(strCurrentMessage, 50, "%s knocked over a jug of lemonade.", 
							  causer, sQuantityCups);
				else
					sprintf_s(strCurrentMessage, 50, "%s knocked over %i cups of lemonade.", 
							  causer, sQuantityCups);
				// end of if/else statements

				// update the number of lemonade cups knocked over by a child today
				financeDaily.cupsDestroyed += sQuantityCups;
			break;

			default: cout<<"Error with 'itemUsed' object. Value found: "<<	itemUsed<<endl;
			
		} // end of switch statement

		// add the chosen message to the store message
		AddStoreMessage(strCurrentMessage);

	} // end of if for stockAvailable

} // end of DestroyStock function
