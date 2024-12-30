#include "pch.h"
#include "AncientBeltData.h"
#include "CharacterInfo.h"

AncientBeltData::AncientBeltData()
{
	_itemLoadPath = DATA_ADDR_ITEM;
	_itemLoadPath += L"Equipment/Belt/Warrior/AncientBeltData.xml";

	_itemSavePath = _itemLoadPath;
	LoadItemInformationFromFile(_itemLoadPath);
}

AncientBeltData::~AncientBeltData()
{
}

bool AncientBeltData::ItemEffectToSelf(bool active)
{
	if (_itemOwnerUnitInfo == nullptr)
	{
		return false;
	}
	else
	{
		if (active)
		{
			_itemOwnerUnitInfo->UpdateAddInformation(shared_from_this(), true);
		}
		else
		{
			_itemOwnerUnitInfo->UpdateAddInformation(shared_from_this(), false);
		}
	}

	return true;
}
