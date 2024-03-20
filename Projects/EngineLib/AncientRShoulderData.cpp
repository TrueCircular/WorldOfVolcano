#include "pch.h"
#include "AncientRShoulderData.h"
#include "CharacterInfo.h"

AncientRShoulderData::AncientRShoulderData()
{
	_itemLoadPath = DATA_ADDR_ITEM;
	_itemLoadPath += L"Equipment/RightShoulder/Warrior/AncientRShoulderData.xml";

	_itemSavePath = _itemLoadPath;
	LoadItemInformationFromFile(_itemLoadPath);
}

AncientRShoulderData::~AncientRShoulderData()
{
}

bool AncientRShoulderData::ItemEffectToSelf(bool active)
{
	if (_itemOwnerUnitInfo.lock() == nullptr)
	{
		return false;
	}
	else
	{
		if (active)
		{
			_itemOwnerUnitInfo.lock()->UpdateAddInformation(shared_from_this(), true);
		}
		else
		{
			_itemOwnerUnitInfo.lock()->UpdateAddInformation(shared_from_this(), false);
		}
	}

	return true;
}
