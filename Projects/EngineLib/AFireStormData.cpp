#include "pch.h"
#include "AFireStormData.h"

AFireStormData::AFireStormData()
{
	wstring LoadPath = DATA_ADDR_ABILITY;
	LoadPath += L"Ragnaros/FireStorm.xml";

	LoadAbilityDataFromFile(LoadPath);
}

AFireStormData::~AFireStormData()
{
}
