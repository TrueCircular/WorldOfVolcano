#include "pch.h"
#include "AManaPotionData.h"

AManaPotionData::AManaPotionData()
{
	wstring LoadPath = DATA_ADDR_ABILITY;
	LoadPath += L"Common/ManaPotion.xml";

	LoadAbilityDataFromFile(LoadPath);
}

AManaPotionData::~AManaPotionData()
{
}