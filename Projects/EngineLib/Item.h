#pragma once

class ItemData;
class CharacterInfo;

class Item : public GameObject
{
	using Super = GameObject;
public:
	Item(){}
	virtual ~Item(){}
protected:
	shared_ptr<ModelRenderer>	_itemModel;
	shared_ptr<ItemData>		_itemData;
	shared_ptr<GameObject>		_itemOwner;
	shared_ptr<CharacterInfo>   _itemOwnerInfo;
public:
	void SetItemOwner(const shared_ptr<GameObject>& owner) { _itemOwner = owner; }
	const shared_ptr<ItemData>& GetItemData() const { return _itemData; }
public:
	virtual bool Init(const shared_ptr<GameObject>& owner) { return false; }
	virtual bool ApplyItem(const bool& apply) { return false; }
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
};

