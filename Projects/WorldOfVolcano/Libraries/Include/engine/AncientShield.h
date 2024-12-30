#pragma once
#include "Item.h"

class AncientShield : public Item
{
	using Super = Item;
public:
	AncientShield();
	virtual ~AncientShield();
public:
	virtual bool Init(const shared_ptr<GameObject>& owner) override;
	virtual bool ApplyItem(const bool& apply) override;
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
};

