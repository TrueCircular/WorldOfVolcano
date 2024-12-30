#pragma once
#include "EnemyUnit.h"

class MoltenGiant : public EnemyUnit
{
	using Super = EnemyUnit;
public:
	MoltenGiant();
	virtual ~MoltenGiant();
protected:
	virtual void CharacterInit() override;
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
};

