#include "pch.h"
#include "Camera.h"

Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera() : Super(Component(ComponentType::Camera))
{
}

Camera::~Camera()
{
}

void Camera::SetCameraMinMaxRotationYaw(float min, float max)
{
	_camMinYaw = ::XMConvertToRadians(min);
	_camMaxYaw = ::XMConvertToRadians(max);
}

void Camera::SetCameraMinMaxRotationPitch(float min, float max)
{
	_camMinPitch = ::XMConvertToRadians(min);
	_camMaxPitch = ::XMConvertToRadians(max);
}

void Camera::SetCameraMinMaxRotationRoll(float min, float max)
{
	_camMinRoll = ::XMConvertToRadians(min);
	_camMaxRoll = ::XMConvertToRadians(max);
}

void Camera::SetProjectionType(ProjectionType type)
{
	_projType = type;

	switch (_projType)
	{
	case ProjectionType::Perspective:
	{
		S_MatProjection = _matProjection = ::XMMatrixPerspectiveFovLH(_fov, (_width / _height), _near, _far);
	}break;
	case ProjectionType::Orthographic:
	{
		S_MatProjection = _matProjection = ::XMMatrixOrthographicLH(_width, _height, _near, _far);
	}break;
	}
}

void Camera::UpdateDefaultView()
{
	Vec3 Eye, Look, Up;
	_camPos = GetTransform()->GetPosition();
	_camlook = GetTransform()->GetLookVector();
	_camUp = GetTransform()->GetUpVector();

	Eye = _camPos;
	Look = _camPos + _camlook;
	Up = _camUp;

	S_MatView = _matView = ::XMMatrixLookAtLH(Eye, Look, Up);
	UpdateVector();
}

void Camera::UpdateTargetView()
{
	if (GetTransform()->GetParent() == nullptr)
	{
		UpdateDefaultView();
	}
	else
	{
		Vec3 Eye, Look, Up;
		_camPos = GetTransform()->GetPosition();
		_camUp = GetTransform()->GetUpVector();
		_targetPos = GetTransform()->GetParent()->GetPosition();

		Eye = _camPos;
		Look = _targetPos;
		Up = _camUp;

		S_MatView = _matView = ::XMMatrixLookAtLH(Eye, Look, Up);
		UpdateVector();
	}
}

void Camera::RotateAroundToTarget(const Vec3& pos, const Vec3& axis, float dist)
{
	float angle = axis.Length();

	if (angle > 0.f)
	{
		_camYaw += axis.y;
		_camPitch += axis.x;
		_camPitch = max(_camMinPitch, min(_camMaxPitch, _camPitch));

		Quaternion qt = Quaternion::CreateFromYawPitchRoll(_camYaw, _camPitch, 0.f);

		Vec3 WorldUp, WorldLook;
		Vec3 localUp = Vec3(0,1,0);
		Vec3 localLook = Vec3(0,0,1);

		Vec3 targetPos = GetTransform()->GetParent()->GetPosition();
		Vec3 mPos = pos;
		Vec3 zmrl = targetPos - mPos;

		WorldUp = Vec3::Transform(localUp, qt);
		WorldLook = Vec3::Transform(localLook, qt);

		_distance = dist;
		_camPos = targetPos - (WorldLook * dist);

		GetTransform()->SetPosition(_camPos);
		UpdateMatrix();

		wstring dString = L"Around Dist :";
		dString += ::to_wstring(dist);
		dString += L"\n";
		OutputDebugString(dString.c_str());
	}
}

void Camera::UpdateMatrix()
{
	switch (_camType)
	{
	case CameraType::Debug:
	case CameraType::Normal: 
	{
		UpdateDefaultView();
	}break;
	case CameraType::Target:
	{
		UpdateTargetView();
	}break;
	}
}

void Camera::UpdateVector()
{
	_camRight.x = _matView._11;
	_camRight.y = _matView._21;
	_camRight.z = _matView._31;

	_camUp.x = _matView._12;
	_camUp.y = _matView._22;
	_camUp.z = _matView._32;

	_camlook.x = _matView._13;
	_camlook.y = _matView._23;
	_camlook.z = _matView._33;

	_camlook.Normalize(_camlook);
}

void Camera::Init(CameraType camType, ProjectionType projType, float dist)
{
	_camType = camType;
	_projType = projType;
	_distance = dist;
	_width = static_cast<float>(g_gameDesc.width);
	_height = static_cast<float>(g_gameDesc.height);

	SetCameraMinMaxRotationPitch(-89.f, 89.f);

	switch (_camType)
	{
	case CameraType::Debug:
	case CameraType::Normal:
	{
		UpdateDefaultView();
	}break;
	case CameraType::Target:
	{
		UpdateTargetView();
	}break;	
	}
	switch (_projType)
	{
	case ProjectionType::Perspective:
	{
		S_MatProjection = _matProjection = ::XMMatrixPerspectiveFovLH(_fov, (_width / _height), _near, _far);
	}break;
	case ProjectionType::Orthographic:
	{
		S_MatProjection = _matProjection = ::XMMatrixOrthographicLH(_width, _height, _near, _far);
	}break;
	}
}

void Camera::Update()
{
	UpdateMatrix();
}
