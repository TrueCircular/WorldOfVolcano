#include "pch.h"
#include "CameraMove.h"

void CameraMove::Start()
{
	_lastMousePos = MANAGER_INPUT()->GetScreenMousePos();
}

void CameraMove::Update()
{
	_dt = MANAGER_TIME()->GetDeltaTime();

	Vec3 pos = GetGameObject()->GetCamera()->GetCameraPosition();
	Vec3 rot = GetTransform()->GetLocalRotation();
	Vec3 currentMousePos = MANAGER_INPUT()->GetScreenMousePos();

	//Camera Rotation
	{
		{
			if (MANAGER_INPUT()->GetButton(KEY_TYPE::Q))
			{
				Vec3 rot = GetTransform()->GetLocalRotation();
				rot.y -= _dt * 0.5f;
				GetTransform()->SetLocalRotation(rot);
			}
			if (MANAGER_INPUT()->GetButton(KEY_TYPE::E))
			{
				Vec3 rot = GetTransform()->GetLocalRotation();
				rot.y += _dt * 0.5f;
				GetTransform()->SetLocalRotation(rot);
			}
			if (MANAGER_INPUT()->GetButton(KEY_TYPE::Z))
			{
				Vec3 rot = GetTransform()->GetLocalRotation();
				rot.x += _dt * 0.5f;
				GetTransform()->SetLocalRotation(rot);
			}
			if (MANAGER_INPUT()->GetButton(KEY_TYPE::C))
			{
				Vec3 rot = GetTransform()->GetLocalRotation();
				rot.x -= _dt * 0.5f;
				GetTransform()->SetLocalRotation(rot);
			}
		}
		if (MANAGER_INPUT()->GetButton(KEY_TYPE::RBUTTON))
		{
			float deltaX = currentMousePos.x - _lastMousePos.x;
			float deltaY = currentMousePos.y - _lastMousePos.y;

			rot.y -= deltaX * _dt * 0.075f;
			rot.x -= deltaY * _dt * 0.075f;

			rot.x = (std::max<float>(min(rot.x, 90.0f), -90.0f));
			GetTransform()->SetLocalRotation(rot);
		}
	}
	//translate
	{
		if (MANAGER_INPUT()->GetButton(KEY_TYPE::W))
		{
			pos += GetGameObject()->GetCamera()->GetCameraLookVector() * _speed * _dt;
		}
		if (MANAGER_INPUT()->GetButton(KEY_TYPE::S))
		{
			pos -= GetGameObject()->GetCamera()->GetCameraLookVector() * _speed * _dt;
		}
		if (MANAGER_INPUT()->GetButton(KEY_TYPE::A))
		{
			pos -= GetGameObject()->GetCamera()->GetCameraRightVector() * _speed * _dt;
		}
		if (MANAGER_INPUT()->GetButton(KEY_TYPE::D))
		{
			pos += GetGameObject()->GetCamera()->GetCameraRightVector() * _speed * _dt;
		}
	}

	GetGameObject()->GetCamera()->SetCameraPosition(pos);
	_lastMousePos = currentMousePos;
}
