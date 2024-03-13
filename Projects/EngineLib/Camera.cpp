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

	Eye = _camPos;
	Look = _camPos + _camlook;
	Up = _camUp;

	S_MatView = _matView = ::XMMatrixLookAtLH(Eye, Look, Up);
	UpdateVector();
}

void Camera::UpdateTargetView()
{
	if (_targetTransform == nullptr)
	{
		UpdateDefaultView();
	}
	else
	{
		if(_camIsRotateAround == false)
		{
			_targetPos = _targetTransform->GetPosition();

			Vec3 lookVector = _targetTransform->GetLookVector(); // 타겟의 룩 벡터 가져오기
			lookVector.Normalize(); // 룩 벡터 정규화
			Vec3 newCamPos = _targetPos + lookVector * (-_distance) + Vec3(0, 50.f, 0);
			_camPos = (newCamPos);


			Vec3 Eye, Look, Up;
			Eye = _camPos;
			Look = _targetPos;
			if (_camOffset != Vec3::Zero)
			{
				Look += _camOffset;
			}
			Up = Vec3(0, 1, 0);

			S_MatView = _matView = ::XMMatrixLookAtLH(Eye, Look, Up);
			UpdateVector();
		}
		else
		{
			_targetPos = _targetTransform->GetPosition();

			Vec3 Eye, Look, Up;
			Eye = _camPos;
			Look = _targetPos;
			if (_camOffset != Vec3::Zero)
			{
				Look += _camOffset;
			}
			Up = Vec3(0, 1, 0);

			S_MatView = _matView = ::XMMatrixLookAtLH(Eye, Look, Up);
			UpdateVector();
		}
	}
}

void Camera::RotateAroundToTarget(const Vec3& target, const Vec3& axis)
{
	float angle = axis.Length();

	if (angle > 0.f)
	{
		_camIsRotateAround = true;

		Vec3 targetPos = target;
		targetPos += _camOffset;
		{
			Vec3 dirToCamera = _camPos - targetPos;
			dirToCamera.Normalize(dirToCamera);
			// 수평 거리 계산 (XZ 평면에서의 거리)
			float horizontalDistance = sqrt(dirToCamera.x * dirToCamera.x + dirToCamera.z * dirToCamera.z);

			// 높이 차이 계산
			float heightDifference = dirToCamera.y;

			// 아크탄젠트 함수를 사용하여 피치 각도 계산 (라디안 단위)
			float pitchRadians = atan2(heightDifference, horizontalDistance);
			_camPitch = pitchRadians;
		}
		_camYaw += axis.y;
		_camPitch += axis.x;
		_camPitch = max(_camMinPitch, min(_camMaxPitch, _camPitch));

		Quaternion qt = Quaternion::CreateFromYawPitchRoll(_camYaw, _camPitch, 0.f);

		Vec3 WorldUp, WorldLook;
		Vec3 localUp = Vec3(0,1,0);
		Vec3 localLook = Vec3(0,0,1);

		WorldUp = Vec3::Transform(localUp, qt);
		WorldLook = Vec3::Transform(localLook, qt);

		Vec3 distVec = targetPos - _camPos;
		float dist = distVec.Length();

		_camPos = targetPos - (WorldLook * dist);
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

void Camera::Init(const Vec3& camPos, CameraType camType, ProjectionType projType, float distance)
{
	_camPos = camPos;
	_camType = camType;
	_projType = projType;
	_distance = distance;

	_width = static_cast<float>(g_gameDesc.width);
	_height = static_cast<float>(g_gameDesc.height);

	SetCameraMinMaxRotationPitch(-89.f, 89.f);
	SetCameraMinMaxRotationYaw(-179.f, 179.f);

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
