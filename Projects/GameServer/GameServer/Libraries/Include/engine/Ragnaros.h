#pragma once
#include "EnemyUnit.h"

class Ragnaros : public EnemyUnit
{
	using Super = EnemyUnit;
public:
	Ragnaros();
	virtual ~Ragnaros();
protected:
	virtual void CharacterInit() override;
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
};

