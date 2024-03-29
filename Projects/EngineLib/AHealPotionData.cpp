#include "pch.h"
#include "AHealPotionData.h"

AHealPotionData::AHealPotionData()
{
	wstring LoadPath = DATA_ADDR_ABILITY;
	LoadPath += L"Common/HealPotion.xml";

	LoadAbilityDataFromFile(LoadPath);
}

AHealPotionData::~AHealPotionData()
{
}