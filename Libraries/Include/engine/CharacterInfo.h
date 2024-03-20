#pragma once

class ItemData;

class CharacterInfo : public MonoBehaviour
{
public:
	CharacterInfo();
	virtual ~CharacterInfo();
public:
	void SetDefaultCharacterInfo(const CHARACTER_INFO& info) { _defaultInfo = info; }
	void SetAddCharacterInfo(const CHARACTER_ADD_INFO& info) { _addInfo = info; }
	void SetCharacterInfo(const CHARACTER_INFO& info) { _info = info; }
	const CHARACTER_INFO& GetDefaultCharacterInfo() const { return _defaultInfo; }
	const CHARACTER_ADD_INFO& GetAddCharacterInfo() const { return _addInfo; }
	const CHARACTER_INFO& GetCharacterInfo() const { return _info; }
public:
	void InitInformation();
	void UpdateAddInformation(const shared_ptr<ItemData>& data, const bool& equip);
public:
	bool LoadCharacterInformationFromFile(const wstring& loadPath);
	bool SaveCharacterInformationFromFile(const wstring& savePath);
private:
	CHARACTER_INFO				_defaultInfo;
	CHARACTER_ADD_INFO			_addInfo;
	CHARACTER_INFO				_info;
};