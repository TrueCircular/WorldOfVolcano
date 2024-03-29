#pragma once
#include "UnitStrategy.h"
#include "CoreHoundPattern.h"
#include "MoltenGiantPattern.h"
#include "BaronGeddonPattern.h"
#include "RagnarosPattern.h"

#pragma region Forward Declaration
class CoreHound;
class MoltenGiant;
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
		auto stand = make_shared<CoreHoundStand>();
		strategyList.push_back(stand);

		auto damaged = make_shared<CoreHoundDamaged>();
		strategyList.push_back(damaged);

		auto dead = make_shared<CoreHoundDead>();
		strategyList.push_back(dead);

		auto trace = make_shared<CoreHoundTrace>();
		strategyList.push_back(trace);
		
		auto moveToSpwanPoint = make_shared<CoreHoundMoveToSpwanPoint>();
		strategyList.push_back(moveToSpwanPoint);

		auto battle = make_shared<CoreHoundBattle>();
		strategyList.push_back(battle);

		auto attack = make_shared<CoreHoundAttack>();
		strategyList.push_back(attack);
	}
	else if (std::is_same_v<T, MoltenGiant>)
	{
		auto stand = make_shared<MoltenGiantStand>();
		strategyList.push_back(stand);

		auto damaged = make_shared<MoltenGiantDamaged>();
		strategyList.push_back(damaged);

		auto stun = make_shared<MoltenGiantStun>();
		strategyList.push_back(stun);

		auto dead = make_shared<MoltenGiantDead>();
		strategyList.push_back(dead);

		auto trace = make_shared<MoltenGiantTrace>();
		strategyList.push_back(trace);

		auto moveToSpwanPoint = make_shared<MoltenGiantMoveToSpwanPoint>();
		strategyList.push_back(moveToSpwanPoint);

		auto battle = make_shared<MoltenGiantBattle>();
		strategyList.push_back(battle);

		auto attack = make_shared<MoltenGiantAttack>();
		strategyList.push_back(attack);
	}
	else if (std::is_same_v<T, BaronGeddon>)
	{
		auto stand = make_shared<BaronGeddonStand>();
		strategyList.push_back(stand);

		auto damaged = make_shared<BaronGeddonDamaged>();
		strategyList.push_back(damaged);

		auto dead = make_shared<BaronGeddonDead>();
		strategyList.push_back(dead);

		auto trace = make_shared<BaronGeddonTrace>();
		strategyList.push_back(trace);

		auto moveToSpwanPoint = make_shared<BaronGeddonMoveToSpwanPoint>();
		strategyList.push_back(moveToSpwanPoint);

		auto battle = make_shared<BaronGeddonBattle>();
		strategyList.push_back(battle);

		auto attack = make_shared<BaronGeddonAttack>();
		strategyList.push_back(attack);

		auto ability = make_shared<BaronGeddonAbility>();
		strategyList.push_back(ability);
	}
	else if (std::is_same_v<T, Ragnaros>)
	{
		auto event1 = make_shared<RagnarosEncounterEvent1>();
		strategyList.push_back(event1);

		auto event2 = make_shared<RagnarosEncounterEvent2>();
		strategyList.push_back(event2);

		auto stand = make_shared<RagnarosStand>();
		strategyList.push_back(stand);

		auto stun = make_shared<RagnarosStun>();
		strategyList.push_back(stun);

		auto dead = make_shared<RagnarosDead>();
		strategyList.push_back(dead);

		auto battle = make_shared<RagnarosBattle>();
		strategyList.push_back(battle);

		auto attack = make_shared<RagnarosAttack>();
		strategyList.push_back(attack);

		auto ability1 = make_shared<RagnarosAbility1>();
		strategyList.push_back(ability1);

		auto ability2 = make_shared<RagnarosAbility2>();
		strategyList.push_back(ability2);
	}


	return strategyList;
}