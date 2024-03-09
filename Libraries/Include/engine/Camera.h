#pragma once
#include "Component.h"

enum class ProjectionType
{
	Perspective,
	Orthographic
};

class Camera : public Component
{
	using Super = Component;
public:
	Camera();
	virtual ~Camera();
//member
private:
	ProjectionType _type = ProjectionType::Perspective;
	CameraType _camType = CameraType::Normal;
private:
	Matrix _matView = Matrix::Identity;
	Matrix _matProjection = Matrix::Identity;
	Matrix _matAround = Matrix::Identity;
	Vec3 _pPos = Vec3(0.f);
	Vec3 _lastPos = Vec3(0.f);
	float _near = 0.1f;
	float _far = 10000.f;
	float _fov = XM_PI / 4.f;
	float _width = 0.f;
	float _height = 0.f;
private:
	Vec3 _eye;
	Vec3 _look;
	Vec3 _up = Vec3(0, 1, 0);
private:
	shared_ptr<Transform> _targetTransform;
	Vec3 _target;
public:
	static Matrix S_MatView;
	static Matrix S_MatProjection;
public:
	void SetTargetTransform(const shared_ptr<Transform>& target) { _targetTransform = target; }
	void SetProjectionType(ProjectionType type) { _type = type; }
	void SetNear(float value) { _near = value; }
	void SetPar(float value) { _far = value; }
	void SetFov(float value) { _fov = value; }
	void SetWidth(float value) { _width = value; }
	void SetHeight(float value) { _height = value; }
	void SetCameraType(CameraType type) { _camType = type; }
public:
	ProjectionType GetProjectionType() const { return _type; }
	Matrix& GetViewMatrix() { return _matView; }
	Matrix& GetProjectionMatrix() { return _matProjection; }
private:
	void UpdateDefaultView();
	void UpdateTargetView();
public:
	void RotateAroundTarget(const Vec3& target, const Vec3& axis);
public:
	void UpdateMatrix();
public:
	virtual void Update() override;
};

