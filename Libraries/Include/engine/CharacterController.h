#pragma once
#include "MonoBehaviour.h"

class CharacterController : public MonoBehaviour
{
public:
	CharacterController(){}
	virtual ~CharacterController(){}
protected:
	Vec3 _spwanPos = Vec3(0.f);
public:
	//Setter
	void SetSpwanPosition(const Vec3& pos) { _spwanPos = pos; }

	//Getter
	const Vec3& GetSpwanPosition() const { return _spwanPos; }
public:
	virtual void TakeDamage(const shared_ptr<GameObject>& sender, uint16 damage) {}
};

