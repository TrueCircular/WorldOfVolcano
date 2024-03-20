#pragma once

class TestAbilityScene : public Scene
{
public:
	TestAbilityScene() {}
	virtual ~TestAbilityScene() {}
public:
	virtual void Init() override;
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
};

