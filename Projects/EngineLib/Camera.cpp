#include "pch.h"
#include "Camera.h"

Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera() : Super(Component(ComponentType::Camera))
{
	_width = static_cast<float>(g_gameDesc.width);
	_height = static_cast<float>(g_gameDesc.height);
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
	if (_targetTransform == nullptr)
	{
		return;
	}
	else
	{
		_targetPos = _targetTransform->GetPosition();
		Vec3 eyeDir = _targetPos - _camPos;
		eyeDir.Normalize(eyeDir);

		Vec3 desiredPos = _targetPos - (eyeDir * _camDist);
		Vec3 toDesiredPos = desiredPos - _camPos;

		float length = toDesiredPos.Length();

		if (length > 0.0f)
		{
			Vec3 move;
			toDesiredPos.Normalize(move);

			move *= min(length, 30.f);
			_camPos += move;
		}

		if (MANAGER_INPUT()->GetButton(KEY_TYPE::LBUTTON))
		{

			float dx = _currentMousePos.y - _lastMousePos.y;
			float dy = _currentMousePos.x - _lastMousePos.x;
			float dz = 0.f;

			dx = ::XMConvertToRadians(dx) * 0.1f;
			dy = ::XMConvertToRadians(dy) * 0.1f;

			Quaternion qmat = Quaternion::CreateFromYawPitchRoll(dy, dx, dz);

			_camPos = Vec3::Transform(_camPos, qmat);
		}


		Vec3 Eye, Look, Up;
		Eye = _camPos;
		Look = _targetPos;
		Up = _camUp;

		S_MatView = _matView = ::XMMatrixLookAtLH(Eye, Look, Up);

		//Camera Right, Up, Look Update
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
}

void Camera::RotateAroundToTarget(const Vec3& axis)
{
	float angle = axis.Length();
	Vec3 normalAxis;
	axis.Normalize(normalAxis);

	if (angle > 0)
	{
		_camQRotation = Quaternion::CreateFromYawPitchRoll(axis.y, axis.x, axis.z);
	}
}

void Camera::UpdateMatrix()
{
	_currentMousePos = MANAGER_INPUT()->GetScreenMousePos();

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

	_lastMousePos = _currentMousePos;
}

void Camera::Init(const Vec3 CamPos, CameraType camType, ProjectionType projType, const shared_ptr<Transform> targetTransform, float dist)
{
	_camPos = CamPos;
	_camType = camType;
	_projType = projType;
	_targetTransform = targetTransform;
	_defaultCameDist = dist;
	_camDist = _defaultCameDist;

	UpdateMatrix();
}

void Camera::Update()
{
	UpdateMatrix();
}
