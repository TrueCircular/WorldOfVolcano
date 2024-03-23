#pragma once
#include "MonoBehaviour.h"

class CharacterController : public MonoBehaviour
{
public:
	CharacterController(){}
	virtual ~CharacterController(){}
protected:
	Vec3	_spawnPos = Vec3(0.f);
	AIType	_type = AIType::None;
public:
	bool _isBattle = false;
	bool _isAttack = false;
	bool _isAbility = false;
	bool _isDamaged = false;
	bool _isAlive = true;
public:
	//Setter
	void SetSpawnPosition(const Vec3& pos) { _spawnPos = pos; }
	void SetAIType(AIType type) { _type = type; }
	//Getter
	const Vec3& GetSpawnPosition() const { return _spawnPos; }
	const AIType& GetAiType() const { return _type; }
protected:
	virtual void InitController(){}
public:
	virtual void TakeDamage(const shared_ptr<GameObject>& sender, float damage) {}
	virtual void Respawn(const Vec3& spawnPos = Vec3(0.f)){}
	virtual void DeadEvent(){}
};

