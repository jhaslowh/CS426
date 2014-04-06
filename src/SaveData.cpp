#include "SaveData.h"


SaveData::SaveData()
{
	purchasedItems = new bool[StoreItems::STORE_ITEM_COUNT];
	for (int i = 0; i < StoreItems::STORE_ITEM_COUNT; i++)
		purchasedItems[i] = false;
	purchasedItems[StoreItems::SID_PLAYER_WEAPON_LASER] = true;
	humanAbductCount = 0;
	animalAbductCount = 0;
	playerWeapon1 = 0;
	playerWeapon2 = -1;
	ufoWeapon1 = -1;
	ufoWeapon2 = -1;
	levelToLoad = "none";
}

SaveData::~SaveData()
{
	delete purchasedItems;
}

// Set the specificed item as purchased 
void SaveData::itemPurchsed(int index){
	purchasedItems[index] = true;
}
// Get the state of an item 
bool SaveData::isItemPurchased(int index){
	return purchasedItems[index];
}

// Set human abduct count 
void SaveData::setHumanAbductCount(int value){humanAbductCount = value;}
// Get human abduct count 
int SaveData::getHumanAbductCount(){return humanAbductCount;}
	
// Set Animal abduct count 
void SaveData::setAnimalAbductCount(int value){animalAbductCount = value;}
// Get Animal abduct count 
int SaveData::getAnimalAbductCount(){return animalAbductCount;}
	

// Player starting weapons
void SaveData::setPlayerWeapon1(int value){playerWeapon1 = value;}
int SaveData::getPlayerWeapon1(){return playerWeapon1;}
void SaveData::setPlayerWeapon2(int value){playerWeapon2 = value;}
int SaveData::getPlayerWeapon2(){return playerWeapon2;}

// Ufo starting weapons
void SaveData::setUFOWeapon1(int value){ufoWeapon1 = value;}
int SaveData::getUFOWeapon1(){return ufoWeapon1;}
void SaveData::setUFOWeapon2(int value){ufoWeapon2 = value;}
int SaveData::getUFOWeapon2(){return ufoWeapon2;}

// Change the abduction count of animals by the sent amout
void SaveData::incrAnimalCount(int value){animalAbductCount += value;}

// Change the abduction count of humans by the sent amout
void SaveData::incrHumanCount(int value){humanAbductCount += value;}