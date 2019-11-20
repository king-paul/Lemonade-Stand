/* MainGameLoop.h
 * Programmer: Paul King
 * Date Complete: 4/11/2010
 * Description: This is the header file for MainGameLoop.cpp which holds all the header files and
 *				external variables and object that source file uses. The protoypes for all the
 *				functions called within the file is also located here.
*/

/****************
 * Header Files *
 ****************/
#include <conio.h>
#include <fstream>
#include "Defines.h"

#pragma once; // this header can only be included once by each file that uses it

/**********************************
 * External variables and objects *
 **********************************/
// get external global variables from defines.cpp
extern bool DebugMode;
extern STORE_STATUS eStoreStatus;
extern char cInput;

// get external structure objects created in defines.cpp
extern PlayerData data;
extern PlayerData savedData[];
extern StockItems stock;
extern Recipe recipe;
extern Pricing prices;
extern Finances financeTotal;
extern Weather_Forecast weather;
extern StockList stockDays[];
extern Customers customer;

// get external date and time object
extern SYSTEMTIME systemTime;

/***********************
 * Function prototypes *
 ***********************/
// Functions found in MainGameLoop.cpp
void StartGame();
void ShowInstructions();
void ShowAbout();
void DisplayLogo();
void InitialisePlayerData();
GAME_STATE RunGameMenu();
Weather_Forecast GetWeatherForecast();
void ShowWeather();
void SaveGame();
GAME_STATE LoadGame();
// Functions found in Customer.cpp
void ProcessDay();
void EndDay();
// Functions found in Lemonade.cpp
void SeeFinances();
void BuyStock();
void DisplayStockItems();
void DisplayLemonadeCost();
void SetRecipeOrPrice();
void ShowStockList();
void MakeLemonade();
// Functions found in Debug.cpp
void RunDebugMenu();