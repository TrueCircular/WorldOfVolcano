#include "pch.h"
#include "AFireBoltData.h"

AFireBoltData::AFireBoltData()
{
	wstring LoadPath = DATA_ADDR_ABILITY;
	LoadPath += L"BaronGeddon/FireBolt.xml";

	LoadAbilityDataFromFile(LoadPath);
}

AFireBoltData::~AFireBoltData()
{
}
