#include "pch.h"
#include "EquipmentSlot.h"
#include "ItemData.h"

EquipmentSlot::EquipmentSlot()
{
}

EquipmentSlot::~EquipmentSlot()
{
}

shared_ptr<Item> EquipmentSlot::GetEquipmentItem(uint16 slotNum)
{
	if (slotNum < MAX_EQUIPMENT_ITEM_SLOT)
	{
		if (_equipmentSlot[slotNum] != nullptr)
		{
			return _equipmentSlot[slotNum];
		}
	}

	return nullptr;
}

bool EquipmentSlot::EquipmentItem(uint16 slotNum, const shared_ptr<Item> item)
{
	if (slotNum < MAX_EQUIPMENT_ITEM_SLOT)
	{
		if (item != nullptr)
		{
			auto itemInfo =item->GetItemData()->GetItemInfo();
			if (slotNum + 1 == static_cast<uint16>(itemInfo.ItemType))
			{
				_equipmentSlot[slotNum] = item;
				_equipmentSlot[slotNum]->ApplyItem(true);
				return true;
			}
		}
	}

	return false;
}

bool EquipmentSlot::UnEquipmentItem(uint16 slotNum)
{
	if (slotNum < MAX_EQUIPMENT_ITEM_SLOT)
	{
		if (_equipmentSlot[slotNum] != nullptr)
		{
			_equipmentSlot[slotNum]->ApplyItem(false);
			_equipmentSlot[slotNum] = nullptr;
			return true;
		}
	}

	return false;
}
