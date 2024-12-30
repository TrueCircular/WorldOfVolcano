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
