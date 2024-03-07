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
	if (_targetTransform)
	{
		_eye = GetTransform()->GetLocalPosition();
		_eye = Vec3::Transform(_eye, _targetTransform->GetWorldMatrix());
		_target = _targetTransform->GetPosition();
		_target.y += 15.f;
		_up =

		S_MatView = _matView = ::XMMatrixLookAtLH(_eye, _target, _up);

		_eye = Vec3::Transform(_eye, _targetTransform->GetWorldMatrix().Invert());
		GetTransform()->SetLocalPosition(_eye);
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
		Quaternion qRot = Quaternion::CreateFromAxisAngle(normalAxis, angle);
		Matrix mQat = Matrix::CreateFromQuaternion(qRot);

		// 타겟으로 이동하는 행렬 생성
		Matrix toTarget = Matrix::CreateTranslation(-target);

		// 원래 위치로 돌아오는 행렬 생성
		Matrix backTarget = Matrix::CreateTranslation(target);

		// 이동-회전-되돌리기 순서로 행렬을 곱함
		Matrix mFinal = toTarget * mQat * backTarget;

		// 카메라 행렬에 적용
		Matrix camMat = GetTransform()->GetWorldMatrix();
		camMat = mFinal * camMat;

		// 분해하여 카메라의 위치, 회전, 스케일을 업데이트
		Vec3 s, t;
		Quaternion r;
		camMat.Decompose(s, r, t);
		Vec3 roro = Transform::QuatToEulerAngles(r);

		GetTransform()->SetLocalScale(s);
		GetTransform()->SetLocalRotation(roro);
		GetTransform()->SetLocalPosition(t);
		GetTransform()->UpdateTransform();

		// 뷰 행렬 업데이트
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
