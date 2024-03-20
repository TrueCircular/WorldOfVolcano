#include "pch.h"
#include "AncientHelmData.h"
#include "CharacterInfo.h"

AncientHelmData::AncientHelmData()
{
	_itemLoadPath = DATA_ADDR_ITEM;
	_itemLoadPath += L"Equipment/Helm/Warrior/AncientHelmData.xml";

	_itemSavePath = _itemLoadPath;
	LoadItemInformationFromFile(_itemLoadPath);
}

AncientHelmData::~AncientHelmData()
{
}

bool AncientHelmData::ItemEffectToSelf(bool active)
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
