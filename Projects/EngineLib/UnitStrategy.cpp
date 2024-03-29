#include "pch.h"
#include "UnitStrategy.h"
#include "AIController.h"

void UnitStrategy::SetHostTarget(shared_ptr<Transform> hostTarget)
{
	_hostTargetTransform = hostTarget;
}

void UnitStrategy::UpdateInfo(MONSTER_INFO info)
{
	if (auto sharedTransform = _transform.lock())
	{
		_transform.lock()->SetLocalPosition(info._pos);
		_transform.lock()->SetLocalRotation(info._Rotate);
	}
}
