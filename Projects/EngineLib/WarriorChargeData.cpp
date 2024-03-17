#include "pch.h"
#include "WarriorChargeData.h"

WarriorChargeData::WarriorChargeData()
{
	wstring LoadPath = DATA_ADDR_ABILITY;
	LoadPath += L"Warrior/WarriorCharge.xml";

	LoadAbilityDataFromFile(LoadPath);
}

WarriorChargeData::~WarriorChargeData()
{
}
