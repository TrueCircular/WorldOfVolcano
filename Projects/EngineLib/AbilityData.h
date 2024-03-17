#pragma once

class AbilityData
{
public:
	AbilityData();
	virtual ~AbilityData();
protected:
	AbilityDataDesc _desc;
public:
	bool LoadAbilityDataFromFile(const wstring& path);
	bool SaveAbilityDataToFilePath(const wstring& path);
public:
	void SetAbilityData(const AbilityDataDesc& desc) { _desc = desc; }
	const AbilityDataDesc& GetAbilityData() const { return _desc; }
};

