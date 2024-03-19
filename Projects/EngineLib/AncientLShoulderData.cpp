#include "pch.h"
#include "AncientLShoulderData.h"
#include "CharacterInfo.h"

AncientLShoulderData::AncientLShoulderData()
{
	_itemLoadPath = DATA_ADDR_ITEM;
	_itemLoadPath += L"Equipment/LeftShoulder/Warrior/AncientLShoulderData.xml";

	_itemSavePath = _itemLoadPath;
	LoadItemInformationFromFile(_itemLoadPath);
}

AncientLShoulderData::~AncientLShoulderData()
{
}

bool AncientLShoulderData::ItemEffectToSelf(bool active)
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
