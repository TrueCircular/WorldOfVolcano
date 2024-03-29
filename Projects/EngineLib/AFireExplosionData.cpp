#include "pch.h"
#include "AFireExplosionData.h"

AFireExplosionData::AFireExplosionData()
{
	wstring LoadPath = DATA_ADDR_ABILITY;
	LoadPath += L"Ragnaros/FireExplosion.xml";

	LoadAbilityDataFromFile(LoadPath);
}

AFireExplosionData::~AFireExplosionData()
{
}
