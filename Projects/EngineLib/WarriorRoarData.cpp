#include "pch.h"
#include "WarriorRoarData.h"

WarriorRoarData::WarriorRoarData()
{
	wstring LoadPath = DATA_ADDR_ABILITY;
	LoadPath += L"Warrior/WarriorRoar.xml";

	LoadAbilityDataFromFile(LoadPath);
}

WarriorRoarData::~WarriorRoarData()
{
}
