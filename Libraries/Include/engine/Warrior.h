#pragma once
#include "PlayableUnit.h"

#pragma region Declaration
class PlayerController;
class CharacterInfo;
#pragma endregion

class Warrior : public PlayableUnit
{
	using Super = PlayableUnit;
private:
	bool _isOtherPlayer = false;
public:
	Warrior();
	virtual ~Warrior();
protected:
	virtual void CharacterInit() override;
public:
	void SetisOtherPlayer(bool isOther) { _isOtherPlayer = isOther; }
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
};

