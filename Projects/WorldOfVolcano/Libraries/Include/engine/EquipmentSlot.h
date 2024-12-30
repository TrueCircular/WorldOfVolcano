#pragma once
#include "MonoBehaviour.h"
#include "item.h"

#define MAX_EQUIPMENT_ITEM_SLOT (size_t)6

class EquipmentSlot : public MonoBehaviour
{
public:
	EquipmentSlot();
	virtual ~EquipmentSlot();
private:
	array<shared_ptr<Item>, MAX_EQUIPMENT_ITEM_SLOT> _equipmentSlot;
public:
	shared_ptr<Item> GetEquipmentItem(uint16 slotNum);
public:
	bool EquipmentItem(uint16 slotNum, shared_ptr<Item> item);
	bool UnEquipmentItem(uint16 slotNum);
};

