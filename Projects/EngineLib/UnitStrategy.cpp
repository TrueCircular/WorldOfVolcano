#include "pch.h"
#include "UnitStrategy.h"
#include "AIController.h"

void UnitStrategy::UpdateLocalPosition(Vec3 pos)
{
	if (auto sharedTransform = _transform.lock())
	{
		_transform.lock()->SetLocalPosition(pos);
	}
}
