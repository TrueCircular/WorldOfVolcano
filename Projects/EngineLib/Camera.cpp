#include "pch.h"
#include "Camera.h"

Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera() : Super(Component(ComponentType::Camera))
{
	_width = static_cast<float>(g_gameDesc.width);
	_height = static_cast<float>(g_gameDesc.height);

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

Camera::~Camera()
{
}


void Camera::UpdateDefaultView()
{
	Vec3 Eye, Look, Up;

	Eye = _camPos;
	Look = _camPos + _camlook;
	Up = _camUp;

	S_MatView = _matView = ::XMMatrixLookAtLH(Eye, Look, Up);

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
}

void Camera::UpdateTargetView()
{
	Vec3 Eye, Look, Up;

	Eye = GetTransform()->GetPosition();
	Look = GetTransform()->GetParent()->GetPosition();
	Up = GetTransform()->GetUpVector();

	S_MatView = _matView = ::XMMatrixLookAtLH(Eye, Look, Up);
	//UpdateVector();
}

void Camera::RotateAroundToTarget(const Vec3& axis)
{
	float angle = axis.Length();

	if (angle > 0)
	{
		_cameraYaw += axis.y;
		_cameraPitch += axis.x;
		float minPitch = ::XMConvertToRadians(-89.f);
		float maxPitch = ::XMConvertToRadians(89.f);

		_cameraPitch = max(minPitch, min(maxPitch, _cameraPitch));

		Quaternion qt = Quaternion::CreateFromYawPitchRoll(_cameraYaw, _cameraPitch, 0.f);

		Vec3 WorldUp, WorldLook;
		Vec3 localUp = Vec3(0,1,0);
		Vec3 localLook = Vec3(0,0,1);

		Vec3 targetPos = GetTransform()->GetParent()->GetPosition();
		Vec3 mPos = GetTransform()->GetPosition();
		Vec3 zmrl = targetPos - mPos;
		float dist = Vec3::Distance(targetPos, mPos);

		WorldUp = Vec3::Transform(localUp, qt);
		WorldLook = Vec3::Transform(localLook, qt);
		_camPos = targetPos - (WorldLook * dist);
		_camPos = Vec3::Transform(_camPos, GetTransform()->GetParent()->GetWorldMatrix().Invert());

		GetTransform()->SetLocalPosition(_camPos);

		Update();
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

void Camera::SetViewMatrix(Vec3 vPos, Vec3 vTarget, Vec3 vUp)
{
	_camPos = vPos;
	_targetPos = vTarget;
	S_MatView = _matView = ::XMMatrixLookAtLH(_camPos, _targetPos, vUp);
	UpdateVector();
}

void Camera::Init(const Vec3 CamPos, CameraType camType, ProjectionType projType, const shared_ptr<Transform> targetTransform, float dist)
{
	_camPos = CamPos;
	_camType = camType;
	_projType = projType;
	_targetTransform = targetTransform;
	_targetPos = _targetTransform->GetPosition();
	//_defaultCameDist = dist;
	//_camDist = _defaultCameDist;

	SetViewMatrix(_camPos, _targetPos, _camUp);

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
