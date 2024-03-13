#include "pch.h"
#include "SpringArm.h"

SpringArm::SpringArm()
{
}

SpringArm::~SpringArm()
{
}

void SpringArm::Update()
{
	if (GetTransform() != nullptr && _camera != nullptr)
	{
		_position = GetTransform()->GetPosition();
	}
}
