/* Lemonade Stand V3
 * MainGameLoop.cpp
 * Programmer: Paul King
 * Last Updated: 4/11/2010
 * Description: This file contains the 'main' function which drives the program as soon as it's
 *				executed. Functions for the main menu, in-game menu, instructions, about,
 *				load game, save game are all found withing this file. The menu's also loop until
 *				the user decides to exit them.
 *				Function prototypes, external variables and objects and other header files that
 *				this source file uses can be found in the "MainGameLoop.h" header file.
*/

#include "MainGameLoop.h"

// global enumerator variable for the game state
GAME_STATE eGameState = Initialising;

// main function that runs when program starts
int main(int argc, const char*argv[]) // start of main function
{
	/*******************************************
	 * The main menu goes within this function *
	 *******************************************/
	srand((int)time(NULL)); // generate new random numbers each time the program is run
	GetSystemTime(&systemTime); // get the date and time from the computer system 

	// declare local variables
	int cInput; // number entered by user

	MAIN_MENU eChoice;
	
	// display introduction screen
	cout<<endl<<endl;
	DisplayLogo();
	cout<<"\t\t\t\t    Version 1.13\n\n"
		<<"\t\t\t\t     Created By\n"
		<<"\t\t\t\t      Paul King\n"
		<<"\t\t\t\t        2010"
		<<endl;
	PauseAndClear();

	while(eGameState != Ending)
	{
		if(DebugMode == true)
			cout<<"Debug Mode is on.\n"<<endl;
		// end of if

		// display main menu screen
		DisplayLogo();
		cout<<"Main Menu\n"
			<<"---------\n"
			<<"(1) Start New Game\n"
			<<"(2) Load Saved Game\n"
			<<"(3) View Instructions\n"
			<<"(4) About The Program\n"
			<<"(0) Exit Program\n"
			<<"Select an option: ";

		cInput = getchar(); // get the first character entered
		fflush(stdin);
		
		eChoice = (MAIN_MENU)(cInput-48);

		switch(eChoice)
		{
			case New_Game:
				eGameState = Initialising;
				StartGame();				
			break;

			case Load_Game:
				eGameState = LoadGame();
				if(eGameState == Running)
					StartGame();
				// end of if
			break;

			case Instructions:
				ShowInstructions(); // call function that displays the instructions
			break;

			case About:
				ShowAbout(); // call function that displays the credits
			break;
			
			case Exit:
				eGameState = Ending;
			break;

			default:
				cout<<"\nThat was not a valid response."<<endl;
				PauseAndClear();

		} // end of switch

	} // end of while loop

	return 0; // exits the program

} // end of main function

// function that displays the instreuctions
void ShowInstructions()
{
	// declare local variable that determines whether the next line is being read
	bool blReadPage = true;
	int iPageNum = 1,
		iPagesTotal = 10;

	fstream fsInsructions; // create file tream object	
	char cTextDisplay[256][100]; // create array to store file text

	fsInsructions.open("instructions.txt", ios::in);

	system("cls"); // claer the screen

	if(fsInsructions.is_open())
	{
		// while there is text left in the file
		while(fsInsructions.getline(cTextDisplay[iPageNum], 100))
		{
			// display page heading
			cout<<"\t\t\t\tInstructions\t\t\tPage "<<iPageNum<<" of "<<iPagesTotal<<endl
				<<"\t\t\t\t------------"<<endl;

			for(int line=0; blReadPage == true; line++)
			{
				fsInsructions.getline(cTextDisplay[line], 100);
				
				// if the text read is --EndPage--
				if(strcmp(cTextDisplay[line], "--End Page--") == 0) 
					blReadPage = false;
				else
					cout<<cTextDisplay[line]<<endl; // print the next line of text
				// end of if/else

			} // end of for loop

			iPageNum++;
			PauseAndClear();

			//system("cls");
			blReadPage = true;

		} // end of while loop
	}
	else
	{
		cout<<"There was an error opening instructions.txt. Please check that the file is not\n"
			<<"missing and that is had not been corrupted."<<endl;
	}

} // end of showInstructions functions	

// function that shows the about information
void ShowAbout()
{
	system("cls");

	DisplayLogo();
	cout<<"\t\t\t\tVersion 1.13\n"
		<<"\t\t\t\t------------\n"
		<<"\t\t\t     (C) 2010 Paul King\n"
		<<"\nThis program is student work and was created as an assignment for the\n"
		<<"Academy of Interactive Entertainment in Australia.\n\n"
		<<"This program is designed to run on windows only. Do not Attempt to open\n"
		<<"this file on another operating system.\n"
		<<endl;
	
	PauseAndClear(); // wait for keypress then clear the screen
} // end of showAbout function

// function that is called when the game has started
void StartGame()
{
	/**********************************************
	 * The in game menu goes within this function *
	 **********************************************/
	if(eGameState == Initialising)
	{
		system("cls"); // clear the screen
		cout<<"Please enter your name (MAX 16 Chars): ";
		cin.getline(data.playerName, 16);

		do
		{
			cout<<"\nDifficulty\tStarting money\tInitial Day length\n"
				<<"----------\t--------------\t------------------\n"
				<<"(1) Easy \t      $"<<STARTING_MONEY_EASY<<"\t      "<<STARTING_LENGTH_EASY<<" secs\n"
				<<"(2) Medium \t      $"<<STARTING_MONEY_MEDIUM<<"\t      "<<STARTING_LENGTH_MEDIUM<<" secs\n"
				<<"(3) Hard \t      $"<<STARTING_MONEY_HARD<<"\t      "<<STARTING_LENGTH_HARD<<" secs\n"
				<<"Select your game difficulty: ";
			cInput = getchar();
			fflush(stdin);

			switch(cInput)
			{
				case '1':
					data.currentMoney = STARTING_MONEY_EASY;
					data.dayLength = STARTING_LENGTH_EASY;
					data.dayLengthIncrease = 60;
				break;
				case '2': 
					data.currentMoney = STARTING_MONEY_MEDIUM;
					data.dayLength = STARTING_LENGTH_MEDIUM;
					data.dayLengthIncrease = 40;
				break;
				case '3':
					data.currentMoney = STARTING_MONEY_HARD;
					data.dayLength = STARTING_LENGTH_HARD;
					data.dayLengthIncrease = 10;
				break;

				default: cout<<"\nThat response was invalid. Please try again."<<endl;

			} // end of switch statement

		} while((cInput !='1') && (cInput !='2') && (cInput !='3')); // end of do while loop

		do
		{
			// get the target money from the user
			cout<<"\nWinning Total\n"
				<<"Game Length \t Target\n"
				<<"------------- \t ------\n"
				<<"(1) - Short \t $"<<TARGET_MONEY_SHORT<<endl
				<<"(2) - Medium \t $"<<TARGET_MONEY_MEDIUM<<endl
				<<"(3) - Full \t $"<<TARGET_MONEY_LONG<<endl
				<<"Select your money target: ";
			cInput = getchar();
			fflush(stdin);

			switch(cInput)
			{
				case '1': data.targetMoney = TARGET_MONEY_SHORT;					
					break;

				case '2': data.targetMoney = TARGET_MONEY_MEDIUM;
					break;

				case '3': data.targetMoney = TARGET_MONEY_LONG;
					break;

				default: cout<<"\nThat response was invalid. Please try again."<<endl;

			} // end of switch

		} while((cInput !='1') && (cInput !='2') && (cInput !='3'));

		cout<<"\nYou have selected a starting money value of $"<<data.currentMoney<<endl
			<<"To win you must turn your current money into $"<<data.targetMoney<<endl;
		PauseAndClear();

		InitialisePlayerData(); // call function that sets all starting values	

		// set the game main menu to running
		eGameState = Running;

	} // end of if for eGameState

	// put the day's date into the data structure
	data.date.day = systemTime.wDay;
	data.date.month = systemTime.wMonth;
	data.date.year = systemTime.wYear;
	sprintf_s(data.date.dateString, 11, "%i/%i/%i", data.date.day, data.date.month, data.date.year);

	while(eGameState == Running)
	{
		// get the day's weather using function
		weather = GetWeatherForecast();

		/* Generate random prices for bying stock item within the specified proce range.
		   Numbers are devided by 100 to be converted into dollars for floating point values */
		prices.lemons = (float)(rand()%(PRICE_MAX_LEMON-PRICE_MIN_LEMON) + PRICE_MIN_LEMON) / 100;
		prices.sugarBags = (float)(rand()%(PRICE_MAX_SUGAR-PRICE_MIN_SUGAR) + PRICE_MIN_SUGAR) / 100;
		prices.iceBags = (float)(rand()%(PRICE_MAX_ICE-PRICE_MIN_ICE) + PRICE_MIN_ICE) / 100;

		// call function that brings up the in-game menu and processes it
		eGameState = RunGameMenu();

	} // end of while loop

	cout<<"Thankyou for playing Lemonade Stand"<<endl; // thank player for playing
	PauseAndClear();

} // end of startGame function

// function the show's the in game menu, details above it and handles the input
GAME_STATE RunGameMenu()
{	
	// declare enumerator variables
	GAME_MENU eChoice;

	while(eGameState == Running)
	{
		if(data.currentMoney >= data.targetMoney)
		{
			cout<<"\n\nCongratulations!"<<endl;
			printf("You have made your target of $%.0f and won the game.\n\n", data.targetMoney);
			return Initialising; // exit the function with the game ending value
		}
		// end of if

		// if the player has less than 5 dollars and no lemonade left
		if((data.currentMoney < 5) && (stock.lemonadeJugs < 1)) 
		{
			// if there are not enough ingredients to fill one jug
			if(((stock.lemons < recipe.lemon) && (data.currentMoney < prices.lemons)) ||
			   ((stock.sugarBags < recipe.sugar) && (data.currentMoney < prices.sugarBags)) ||
			   ((stock.iceBags < recipe.ice) && (data.currentMoney < prices.iceBags)) )
			{
				cout<<"\n\nGame Over!\n"
					<<"You have gone bankrupt and lost. Better luck next time.\n"<<endl;

				return Initialising; // end the game and exit the while loop

			} // end of inner if

		} // end of outter if

		system("cls");

		// display the day number
		cout<<"Day No. "<<data.dayNum<<endl;
		ShowWeather(); // call function that displays the weather
		// display money and stock information
		printf("Current Funds: $%.2f\n", data.currentMoney);
		printf("Jugs In Stock: %.2f\n", stock.lemonadeJugs);
		cout<<"Cups In Stock: "<<stock.lemonadeCups<<endl<<endl;

		if(DebugMode == true)
			cout<<"A customer will arrive every "<<customer.delay<<" sconds."<<endl<<endl;
		// end of if

		// display the menu of the game
		cout<<"Game Menu\n"
			<<"---------\n"
			<<"(1) Begin Day\n"
			<<"(2) See Finances\n"
			<<"(3) Buy Stock\n"
			<<"(4) Set Recipe or Price\n"
			<<"(5) Make Lemonade\n"
			<<"(6) Save The Game\n";
		if(DebugMode == true)
			cout<<"(7) Access Debug Tools\n";
		// end of if
		cout<<"(0) Exit Game\n"
			<<"Please enter your choice: "; //prompt user for choice

		cInput = getchar(); // read the first character enterede
		fflush(stdin); // clear all characters from the buffer
			
		eChoice = (GAME_MENU)(cInput-48); // convert the character to the number typed in 

		switch(eChoice)
		{
			case Open_Store:
				// if there is lemonade in stock run the store process
				if((stock.lemonadeCups > 0) && (stock.lemonadeJugs > 0))
				{
					ProcessDay(); // process the day
				}
				else // otherwise disallow the store from opening and display warning message
				{
					cout<<"You must have atleast one jug of lemonade in stock\n"
						<<"to open the store."<<endl;
					PauseAndClear();
				}
				// end of if/else
			break;

			case See_Finances:  // call matching function if 'See Finances' is selected				
				SeeFinances();		
				break;

			case View_Stock: // call matching function if 'Buy Stock' is selscted				
				BuyStock();			
				break;

			case Make_Lemonade: // call matching function if 'Make Lemonade' is selected				
				MakeLemonade();			
				break;

			case Set_Recipe_Or_Price: // call matching function if 'Set Recipe or price' is selected				
				SetRecipeOrPrice();
				break;

			case Save_Game:
				SaveGame();
				break;
				
			case End_Game:				
				cout<<"\nYou have chosen to quit the game."<<endl;
				return Initialising; // the game state becomes ending			
				break;

			case Debug_Menu:
				if(DebugMode == true)
				{
					RunDebugMenu();
					break;
				} // end of if

			default:			
				cout<<"\nThat was not a valid response."<<endl;
				PauseAndClear();

		} // end of switch

	} // end of while loop

	return eGameState;

} // end of RunGameMenu function

// function that generates a random weather forecast and returns it
Weather_Forecast GetWeatherForecast()
{
	// create weather forecast object
	Weather_Forecast weather;

	// create a variable for random rain
	unsigned short sRandomChanceRain;

	// get a random temperature in between the min and max temperatures
	weather.tempCel = ((rand()%(TEMPERATURE_MAX-TEMPERATURE_MIN)) + TEMPERATURE_MIN);

	// calculate the temperature in fahrenheit using the degrees in celcius
	weather.tempFah = (unsigned short)(weather.tempCel * 1.8 + 32);
	
	// get a random weather condition overcast and ...
	weather.overcast = (WEATHER_OVERCAST) (rand()%3);

	// if the temperature is less than 30 o C and the overcast is cloudy
	if((weather.tempCel < 20) && (weather.overcast == Full_Overcast))
	{
		sRandomChanceRain = rand()%100; // get a random number between 1 and 100

		if(DebugMode == true)
			cout<<"Random Rain Chance: "<<sRandomChanceRain<<endl;

		// if the random number greater than or equal to 50
		if(sRandomChanceRain >= 50)			
			weather.rain = true;
		// end of if
	}
	else
	{
		weather.rain = false;
	} 
	// end of if and else

	// determine the temperature condition based on the degrees celsius
	if(weather.tempCel < 20)
	{
		weather.tempRange = Cold;
		customer.delay = 5; // customers will arrive every 5 seconds
	}
	else if((weather.tempCel >= 20) && (weather.tempCel < 30))
	{
		weather.tempRange = Warm;
		customer.delay = 4; // customers will arrive every 4 seconds
	}
	else if((weather.tempCel >= 30) && (weather.tempCel < 40))
	{
		weather.tempRange = Hot;
		customer.delay = 3; // customers will arrive every 3 seconds
	}
	else if(weather.tempCel > 40)
	{
		weather.tempRange = Very_Hot;
		customer.delay = 5; // customers will arrive every 4 seconds
	}
	// end of if/else conditions

	return weather;
	
} // end of GetWeatherForecast function

// function that displays the weather
void ShowWeather()
{
	//display the day and weather
	cout<<"Today's Weather\n"
		<<"---------------\n"
		<<"Temperature: "<<weather.tempCel<<"oC ("<<weather.tempFah<<"oF)\n";

	if(DebugMode == true)
	{
		switch(weather.tempRange)
		{
			case Cold: cout<<"Cold ";
				break;
			case Warm: cout<<"Warm ";
				break;
			case Hot: cout<<"Hot ";
				break;
			case Very_Hot: cout<<"Very Hot ";
				break;
			default: cout<<"Error reading value";

		} // end of switch		

	} // end of if

	cout<<"Cloud Coverage: ";

	switch(weather.overcast)
	{
		case No_Clouds: cout<<"Clear Sky";
			break;
		case Some_Clouds: cout<<"Some Clouds";
			break;
		case Full_Overcast: cout<<"Overcast";
			break;

	} // end of switch statement

	if(weather.rain == true)
		cout<<" and rain"<<endl<<endl;
	else
		cout<<endl<<endl;
	// end of if/else

} // end of ShowWeather function

// function that display the lemonade stand logo text
void DisplayLogo()
{
	cout<<"       _       _____   _    _   ______   __    _     _     ____     _____ \n"
		<<"      | |     |  ___| | \\  / | |  __  | |  \\  | |   / \\   |  _ \\   |  ___|\n"
		<<"      | |     | |___  |  \\/  | | |  | | |   \\ | |  / o \\  | | \\ \\  | |___ \n"
		<<"      | |     |  ___| |      | | |  | | | |\\ \\| | /  _  \\ | |  ) ) |  ___|\n" 
		<<"      | |___  | |___  | |\\/| | | |__| | | | \\   | | | | | | |_/ /  | |___ \n"
		<<"      |_____| |_____| |_|  |_| |______| |_|  \\__| |_| |_| |____/   |_____|\n"
		<<endl
		<<"\t\t    _____   _____    _     __    _   ____	    \n"
		<<"\t\t   |  ___| |_   _|  / \\   |  \\  | | |  _ \\   \n"
		<<"\t\t   | |___    | |   / o \\  |   \\ | | | | \\ \\\n"
		<<"\t\t   |___  |   | |  /  _  \\ | |\\ \\| | | |  ) ) \n"
		<<"\t\t    ___| |   | |  | | | | | | \\   | | |_/ /   \n"
		<<"\t\t   |_____|   |_|  |_| |_| |_|  \\__| |____/     \n"
		<<endl;

} // end of DisplayLogo function

// function that allows the game data to be saved to the savegame file
void SaveGame()
{
	// create an object of for file i/o
	fstream fsLoadFile;
	fstream fsSaveFile;
	
	// declare local variables
	unsigned short sSlotChosen;
	bool blValidation;
	char cStringEntered[3]; // string for user input

	// put all values of game structures into the upper structure
	data.stock = stock;
	data.recipe = recipe;
	data.prices	= prices;
	data.finance = financeTotal;
	data.weather = weather;

	// open the save game file for reading from in binary format
	fsLoadFile.open("savegame.bin", ios::in|ios::out|ios::binary);

	if(fsLoadFile.is_open())
	{
		do
		{
			fsLoadFile.read((char*)&savedData, sizeof(PlayerData)*10); // read data from the file
			
			system("cls");
			
			cout<<"No.\tPlayer Name\tDate Saved\tDay\tCurrent Funds\tTarget Amount"<<endl;

			// load the contents of the file 1 structure at a time
			for(int i=0; i<10; i++)
			{
				if(strlen(savedData[i].playerName) > 0)
				{
					// display information about each save game
					cout<<i+1<<". \t"<<savedData[i].playerName<<" \t"<<savedData[i].date.dateString<<" \t "<<savedData[i].dayNum
						<<" \t    $"<<savedData[i].currentMoney<<" \t    $"<<savedData[i].targetMoney<<endl;
				}
				else
				{
					cout<<i+1<<". \t\t\t\t\tEmpty "<<endl;
				}
				// end of if/else

			} // end of for loop

			fsLoadFile.close(); // close the file
			// open the save game file for writing to in binary format
			fsSaveFile.open("savegame.bin", ios::out | ios::binary);


			cout<<"(1) - Save The Game\n"
				<<"(2) - Exit\n"
				<<"Enter you selection: ";
			cInput = getchar();
			fflush(stdin);

			if(cInput == '1')
			{		
				// ask user which slot to use
				cout<<"\nSelect a slot to save you game in (0 to cancel): ";
				cin>>cStringEntered;
				cin.ignore();

				blValidation = CheckEntryFormat(cStringEntered);
				sSlotChosen = atoi(cStringEntered);

				if((sSlotChosen > 0) && (sSlotChosen <=10)) // if a valid slot was chosen
				{
					if(strlen(savedData[sSlotChosen-1].playerName) > 0)
					{
						cout<<"This slot already contains data. Is it ok to overwrite? (Y)es, (N)o\n";
						cInput = getchar();
						fflush(stdin);

						if((cInput == 'y') || (cInput == 'Y'))
						{
							savedData[sSlotChosen-1] = data;

							// write the stucture to the file in binary format
							fsSaveFile.write((char*)&savedData, sizeof(PlayerData)*10);
							cout<<"The game has been saved."<<endl;
							system("pause");	

							fsSaveFile.sync();

						} // end of inner if
					}
					else
					{
						savedData[sSlotChosen-1] = data;

						// write the stucture to the file in binary format
						fsSaveFile.write((char*)&savedData, sizeof(PlayerData)*10);
						cout<<"The game has been saved."<<endl;
						system("pause");

						fsSaveFile.sync();
						
					} // end of mid if/else
				}
				else if(sSlotChosen != 0)
				{
					cout<<"You must enter a number between 0 and 10"<<endl;
					PauseAndClear();
				} // end of if/else for checking the number entered

			} // end of if for option 1
			else if(cInput == '2')
			{
				system("cls");
			}
			else
			{
				cout<<"That was not a valid response"<<endl;
				PauseAndClear();
			}
			// end of if/else statements for cInput
			
		} while(cInput != '2'); // repeat if the user did choose to close the screen
	} 
	else
	{
		cout<<"Error: Could not open 'savegame.bin'. Please check that the file\n"
		<<"is not missing from the same folder as this game."<<endl;
		PauseAndClear();		

	} // end of if/else for checking if the file is open

	fsSaveFile.close(); // close the file from the stream
	fsLoadFile.clear(); // clear the flag form the file stream can be re-opened later

} // end of SaveGame function

// function the allows user to load a previous saved game state stored in file
GAME_STATE LoadGame()
{
	// declare local objects, variables and arrays
	fstream fsLoadFile;

	char cStringEntered[3]; // string for user input
	unsigned short sGameChosen; // the game slot chosen by the user
	unsigned short sSavesFound = 0; // the number of saves that were found in the file
	bool blValidation; // checks if the data entered by the user was a number
	
	fsLoadFile.open("savegame.bin", ios::in|ios::out|ios::beg|ios::binary); // open file for both reading and writing

	if(fsLoadFile.is_open())
	{
		do
		{
			fsLoadFile.read((char*)&savedData, sizeof(PlayerData)*10); // read data from the file

			system("cls");

			cout<<"No.\tPlayer Name\tDate Saved\tDay\tCurrent Funds\tTarget Amount"<<endl;

			// load the contents of the file 1 structure at a time
			for(int i=0; i<10; i++)
			{
				if(strlen(savedData[i].playerName) > 0)
				{
					// display information about each save game
					cout<<i+1<<". \t"<<savedData[i].playerName<<" \t"<<savedData[i].date.dateString<<" \t "<<savedData[i].dayNum
						<<" \t    $"<<savedData[i].currentMoney<<" \t    $"<<savedData[i].targetMoney<<endl;

					sSavesFound++; // increment the number of saved games found
				}
				else
				{
					cout<<i+1<<".\t\t\t\t\tEmpty"<<endl;
				}
				// end of if/else

			} // end of for loop

			// if no saved games were found
			if(sSavesFound == 0)
			{				
				cout<<"\nThere are no saved games found. ";
				PauseAndClear();

				return Initialising;
			} // end of if
			else
			{
				cout<<"(1) - Load a saved game\n"
					<<"(2) - Delete an existing game\n"
					<<"(3) - Exit to main menu\n"
					<<"Enter your selection: ";
				cInput = getchar();
				fflush(stdin);

				switch(cInput)
				{
					case '1': 
						cout<<"\nSelect a game to load (0 to cancel): ";
						cin>>cStringEntered;
						cin.ignore();
						// check that the entry is numerical
						blValidation = CheckEntryFormat(cStringEntered);
						// convert the string to a short integer
						sGameChosen = atoi(cStringEntered);
						
						if((sGameChosen >=0) && (sGameChosen <=10)) // check that a valid number is chosen
						{
							if(sGameChosen == 0)
							{
								system("cls");
								//return Initialising;
							}
							else if(strlen(savedData[sGameChosen-1].playerName) > 0)
							{
								// load data and run the game
								data = savedData[sGameChosen-1];
								cout<<"The game has been loaded"<<endl;
								PauseAndClear();

								fsLoadFile.close(); // close the file for use later
								fsLoadFile.clear(); // clear the flag form the file stream

								return Running;
							}
							else
							{
								cout<<"That slot does not contain a saved game."<<endl;
								PauseAndClear();
							}			
							// end of inner if/else statements
						}
						else if(blValidation == true)
						{
							cout<<"You must enter a number between 0 and 10"<<endl;
							PauseAndClear();
						}
						// end of outter if/else
					break;

					case '2':						
						cout<<"\nSelect a game to delete(0 to cancel): ";
						cin>>cStringEntered;
						cin.ignore();

						blValidation = CheckEntryFormat(cStringEntered);

						sGameChosen = atoi(cStringEntered);

						if((sGameChosen >=0) && (sGameChosen <=sSavesFound)) // check that a valid number is chosen
						{							
							if((sGameChosen !=0) && (strlen(savedData[sGameChosen-1].playerName) > 0))
							{
								// remove all indexes after the number chosen back one
								for(int i=sGameChosen-1; i<sSavesFound; i++)								
									savedData[i] = savedData[i+1];
								// end of for loop

								savedData[sSavesFound].playerName[0] = '\0'; // remove the name in the last saved game
								sSavesFound--; // decrease the number of saved games by 1

								fsLoadFile.seekg(0, ios::beg); // move marker back to the start of the file							
								
								fsLoadFile.write((char*)&savedData, sizeof(PlayerData)*10); // overite the file with new data

								// syncronize file with contents in the buffer
								flush(fsLoadFile);
								//fsLoadFile.sync();								

								cout<<"The game has been deleted"<<endl;
								PauseAndClear();
							}
							else if(sGameChosen !=0)
							{
								cout<<"That slot does not contain a saved game."<<endl;
								PauseAndClear();
							}
							else
							{
								system("cls");
							}
							// end of inner if/else statements							

						} // end of if
						else
						{
							cout<<"You must select a number between 0 and 10."<<endl;
							PauseAndClear();
						} // end of else
					break;

					case '3': 
						system("cls");						
					break;

					default:
						cout<<"That was not a valid response"<<endl;
						PauseAndClear();

				} // end of switch statement

			} // end of if/else statement					

		} while(cInput != '3'); // do/while loop
	}
	else
	{
		cout<<"Error: Could not open 'savegame.bin'. Please check that the file\n"
			<<"is not missing from the same folder as this game."<<endl;
		PauseAndClear();
	}
	// end of outer if/else

	flush(fsLoadFile);

	fsLoadFile.close(); // close the file for use later	
	fsLoadFile.clear(); // clear the flag form the file stream
	return Initialising;

} // end of LoagGame function