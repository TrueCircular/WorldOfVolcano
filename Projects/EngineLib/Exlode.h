#pragma once
class Exlode : public ParticleObj
{
private:


public:
	virtual void Update() override;
	virtual void LateUpdate() {};

	virtual void OnDestroy(shared_ptr<ParticleInstance>& instance) override;
public:
};
