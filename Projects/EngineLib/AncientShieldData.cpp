#include "pch.h"
#include "AncientShieldData.h"
#include "CharacterInfo.h"

AncientShieldData::AncientShieldData()
{
	_itemLoadPath = DATA_ADDR_ITEM;
	_itemLoadPath += L"Equipment/Shield/AncientShieldData.xml";

	_itemSavePath = _itemLoadPath;
	LoadItemInformationFromFile(_itemLoadPath);
}

AncientShieldData::~AncientShieldData()
{
}

bool AncientShieldData::ItemEffectToSelf(bool active)
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
