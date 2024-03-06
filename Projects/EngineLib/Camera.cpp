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


void Camera::RotateAroundTarget(const Vec3& target, const Vec3& axis)
{
	float angle = axis.Length();
	Vec3 normalAxis;
	axis.Normalize(normalAxis);

	if (angle > 0)
	{
		Quaternion qRot = Quaternion::CreateFromYawPitchRoll(axis.y, axis.x, axis.z);
		Matrix mQat = Matrix::CreateFromQuaternion(qRot);

		Vec3 s, t;
		Quaternion r;
		S_MatView.Decompose(s, r, t);

		Matrix mS, mR, mT, mW;

		mS = Matrix::CreateScale(s);
		mR = Matrix::CreateFromQuaternion(r);
		mT = Matrix::CreateTranslation(t);
		mW = mS * mR * mQat * mT;

		S_MatView = mW;
	}

}

void Camera::UpdateMatrix()
{
	switch (_camType)
	{
	case CameraType::Debug:
	case CameraType::Normal: 
	{
		_eye = GetTransform()->GetPosition();
		_look = _eye + GetTransform()->GetLookVector();
		_up = GetTransform()->GetUpVector();

		S_MatView = _matView = ::XMMatrixLookAtLH(_eye, _look, _up);

	}break;
	case CameraType::Target:
	{
		_eye = GetTransform()->GetPosition();
		_targetVec = Vec3(0.f);
		_up = GetTransform()->GetUpVector();

		S_MatView = _matView = ::XMMatrixLookAtLH(_eye, _targetVec, _up);
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
