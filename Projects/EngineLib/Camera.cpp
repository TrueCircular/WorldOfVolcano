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
	if (_lastPos != Vec3::Zero)
	{
		_target = GetTransform()->GetParent()->GetPosition();
		//_target.y += 15.f;

		float dist = 100.f;
		Vec3 eyeDir = _target - _lastPos;
		eyeDir.Normalize(eyeDir);

		_eye = _target + (-eyeDir * dist);
		//_up = Vec3(0,1,0);
		S_MatView = _matView = ::XMMatrixLookAtLH(_eye, _target, _up);

		Matrix tolocal = GetTransform()->GetParent()->GetWorldMatrix().Invert();
		_eye = Vec3::Transform(_eye, tolocal);

		GetTransform()->SetLocalPosition(_eye);
	}
	else
	{
		_eye = GetTransform()->GetPosition();
		_look = GetTransform()->GetParent()->GetPosition();
		_up = GetTransform()->GetUpVector();

		S_MatView = _matView = ::XMMatrixLookAtLH(_eye, _look, _up);
	}



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

		Vec3 target = GetTransform()->GetParent()->GetPosition();
		Matrix toTarget = Matrix::CreateTranslation(-target);
		Matrix toBack = Matrix::CreateTranslation(target);

		_pPos = GetTransform()->GetLocalPosition();
		_pPos = Vec3::Transform(_pPos, toTarget);
		_pPos = Vec3::TransformNormal(_pPos, mQat);
		_pPos = Vec3::Transform(_pPos, toBack);

		float dt = MANAGER_TIME()->GetDeltaTime();

		_lastPos = Vec3::Lerp(GetTransform()->GetLocalPosition(), _pPos, 150 * dt);
		_lastPos = Vec3::Transform(_lastPos, GetTransform()->GetParent()->GetWorldMatrix());
		//_up = ::XMVector3TransformNormal(_up, mQat);

		UpdateMatrix();
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
	UpdateMatrix();
}
