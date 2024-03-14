#pragma once
class CharacterInfo : public MonoBehaviour
{
public:
	CharacterInfo();
	virtual ~CharacterInfo();
public:
	void SetCharacterInfo(CHARACTER_INFO info) { _info = info; }
	void SetAddCharacterInfo(const CHARACTER_ADD_INFO& info) { _addInfo = info; }
	const CHARACTER_INFO& GetCharacterInfo() const { return _info; }
	const CHARACTER_ADD_INFO& GetAddCharacterInfo() const { return _addInfo; }
public:
	void InitInformation();
public:
	bool LoadCharacterInformationFromFile(const wstring& loadPath);
	bool SaveCharacterInformationFromFile(const wstring& savePath);
private:
	CHARACTER_INFO _info;
	CHARACTER_ADD_INFO _addInfo;
};