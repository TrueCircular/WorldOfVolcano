#include "pch.h"
#include "AWarriorChargeData.h"

AWarriorChargeData::AWarriorChargeData()
{
	wstring LoadPath = DATA_ADDR_ABILITY;
	LoadPath += L"Warrior/WarriorCharge.xml";

	LoadAbilityDataFromFile(LoadPath);
}

AWarriorChargeData::~AWarriorChargeData()
{
}
