#include "pch.h"
#include "StaticCollider.h"

StaticCollider::StaticCollider()
{
}

StaticCollider::~StaticCollider()
{
}

bool StaticCollider::OnCollisionEnter()
{
	return false;
}

bool StaticCollider::OnCollisionExit()
{
	return false;
}

bool StaticCollider::Intersects(const Ray& ray, OUT float& distance)
{
	return Super::Intersects(ray, distance);
}

void StaticCollider::Update()
{
}
