/* Lemonade Stand
 * Debug.cpp
 * Programmer: Paul King
 * Last Updated: 4/11/2010
 * Description: This is the header file for Duebug.cpp which holds all the header files,
 *				external variables and objectsand constants that the source file uses.
 *				The protoypes for all the functions called within the file is also located here.		
*/

#include "defines.h"

// get external global variables from defines.cpp
extern bool DebugMode;
extern STORE_STATUS eStoreStatus;
extern char cInput;

// get external structure objects created in defines.cpp
extern PlayerData data;
extern StockItems stock;
extern Recipe recipe;
extern Pricing prices;
extern Finances finance;
extern Weather_Forecast weather;
extern StockList stockDays[];

/**********************
 * FUNTION PROTOTYPES *
 **********************/
void SetPlayerMoney();
void ChangeStockItem(unsigned short optionType);
void RemoveFromStockList(unsigned short sIndex);
void SetWeatherConditions();
void ShowStockList();