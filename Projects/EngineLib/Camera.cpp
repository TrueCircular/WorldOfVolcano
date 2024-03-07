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
	_eye = GetTransform()->GetPosition();
	_look = _eye + GetTransform()->GetLookVector();
	_up = GetTransform()->GetUpVector();

	S_MatView = _matView = ::XMMatrixLookAtLH(_eye, _look, _up);
}

void Camera::UpdateTargetView()
{
	_eye = GetTransform()->GetPosition();
	_target = GetTransform()->GetParent()->GetPosition();
	_target.y += 15.f;
	_up = GetTransform()->GetUpVector();

	S_MatView = _matView = ::XMMatrixLookAtLH(_eye, _target, _up);
}

void Camera::RotateAroundTarget(const Vec3& target, const Vec3& axis)
{
	float angle = axis.Length();
	Vec3 normalAxis;
	axis.Normalize(normalAxis);

	if (angle > 0)
	{
		// 회전 쿼터니언 생성
		Quaternion qRot = Quaternion::CreateFromYawPitchRoll(axis.y, axis.x, axis.z);
		Matrix mQat = Matrix::CreateFromQuaternion(qRot);

		Vec3 pPos = GetTransform()->GetParent()->GetPosition();

		Matrix toTarget = Matrix::CreateTranslation(-pPos);
		Matrix toBack = Matrix::CreateTranslation(pPos);
		Matrix mFinal = toTarget * mQat * toBack;

		S_MatView = mFinal * S_MatView;

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

	switch (_type)
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
	//UpdateMatrix();
}
