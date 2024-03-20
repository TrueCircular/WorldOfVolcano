#pragma once
#include "UnitStrategy.h"
#include "CoreHoundPattern.h"
#include "MoltenGaintPattern.h"
#include "BaronGeddonPattern.h"
#include "RagnarosPattern.h"

#pragma region Forward Declaration
class CoreHound;
class MoltenGaint;
class BaronGeddon;
class Ragnaros;
#pragma endregion

class StrategyFactory
{
public:
	template<typename T>
	static vector<shared_ptr<UnitStrategy>> GetStrategyList();
};

template<typename T>
inline vector<shared_ptr<UnitStrategy>> StrategyFactory::GetStrategyList()
{
	vector<shared_ptr<UnitStrategy>> strategyList;

	if (std::is_same_v<T, CoreHound>)
	{
		shared_ptr<StandStrategy> stand = make_shared<CoreHoundStand>();
		strategyList.push_back(stand);

	}
	else if (std::is_same_v<T, MoltenGaint>)
	{

	}
	else if (std::is_same_v<T, BaronGeddon>)
	{

	}
	else if (std::is_same_v<T, Ragnaros>)
	{

	}


	return strategyList;
}