#include "pch.h"
#include "AWarriorRoarData.h"

AWarriorRoarData::AWarriorRoarData()
{
	wstring LoadPath = DATA_ADDR_ABILITY;
	LoadPath += L"Warrior/WarriorRoar.xml";

	LoadAbilityDataFromFile(LoadPath);
}

AWarriorRoarData::~AWarriorRoarData()
{
}
