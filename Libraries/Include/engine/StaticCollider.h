#pragma once
#include "SphereCollider.h"

class StaticCollider : public SphereCollider
{
	using Super = SphereCollider;
public:
	StaticCollider();
	virtual ~StaticCollider();
private:
	unordered_set<wstring, GameObject> _targetList;
public:
	bool OnCollisionEnter();
	bool OnCollisionExit();
public:
	virtual bool Intersects(const Ray& ray, OUT float& distance) override;
public:
	virtual void Update() override;
};

