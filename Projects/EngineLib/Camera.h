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
	ProjectionType _projType = ProjectionType::Perspective;
	CameraType _camType = CameraType::Normal;
private:
	float _near = 0.1f;
	float _far = 10000.f;
	float _fov = XM_PI / 4.f;
	float _width = 0.f;
	float _height = 0.f;
private:
	Vec3 _camPos = Vec3(0.f);
	Vec3 _camRight = Vec3(0.f);
	Vec3 _camUp = Vec3(0.f, 1.f, 0.f);
	Vec3 _camlook = Vec3(0.f, 0.f, -1.f);
	Quaternion _camQRotation = Quaternion::Identity;
	float _cameraYaw = 0.f;
	float _cameraPitch = 0.f;
private:
	shared_ptr<Transform>	_targetTransform = nullptr;
	Vec3					_targetPos = Vec3(0.f);
private:
	Matrix _matView = Matrix::Identity;
	Matrix _matProjection = Matrix::Identity;
public:
	static Matrix S_MatView;
	static Matrix S_MatProjection;
public:
	//Camera
	//Setter
	void SetCameraPosition(const Vec3& pos) { _camPos = pos; }
	void SetTargetTransform(const shared_ptr<Transform>& target) { _targetTransform = target; }
	//Getter
	const Vec3& GetCameraPosition() const { return _camPos; }
	const Vec3& GetCameraLookVector() const { return _camlook; }
	const Vec3& GetCameraRightVector() const { return _camRight; }
	const Vec3& GetCameraUpVector() const { return _camUp; }
public:
	//Projection
	//Setter
	void SetNear(float value) { _near = value; }
	void SetPar(float value) { _far = value; }
	void SetFov(float value) { _fov = value; }
	void SetWidth(float value) { _width = value; }
	void SetHeight(float value) { _height = value; }
	void SetCameraType(CameraType type) { _camType = type; }
	void SetProjectionType(ProjectionType type) { _projType = type; }
	void SetYaw(float yaw) { _cameraYaw = yaw; }
	//Getter
	const ProjectionType& GetProjectionType() const { return _projType; }
	const Matrix& GetViewMatrix() const { return _matView; }
	const Matrix& GetProjectionMatrix() const { return _matProjection; }
private:
	void UpdateDefaultView();
	void UpdateTargetView();
	void UpdateMatrix();
	void UpdateVector();
	void SetViewMatrix(Vec3 vPos, Vec3 vTarget, Vec3 vUp);
public:
	void RotateAroundToTarget(const Vec3& axis);
public:
	void Init(const Vec3 CamPos, CameraType camType = CameraType::Normal, ProjectionType projType = ProjectionType::Perspective, const shared_ptr<Transform> targetTransform = nullptr,  float dist = 0.f);
	virtual void Update() override;
};

