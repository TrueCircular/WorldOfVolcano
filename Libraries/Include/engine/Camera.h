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
	ProjectionType	_projType = ProjectionType::Perspective;
	CameraType		_camType = CameraType::Normal;
private:
	float _near = 0.1f;
	float _far = 10000.f;
	float _fov = XM_PI / 4.f;
	float _width = 0.f;
	float _height = 0.f;
	float _distance = 0.f;
private:
	Vec3 _camPos = Vec3(0.f);
	Vec3 _camRight = Vec3(0.f);
	Vec3 _camUp = Vec3(0.f, 1.f, 0.f);
	Vec3 _camlook = Vec3(0.f, 0.f, 1.f);
	Quaternion _camQRotation = Quaternion::Identity;
	float _camYaw = 0.f;
	float _camPitch = 0.f;
	float _camRoll = 0.f;

	float _camMinYaw = 0.f;
	float _camMaxYaw = 0.f;
	float _camMinPitch = 0.f;
	float _camMaxPitch = 0.f;
	float _camMinRoll = 0.f;
	float _camMaxRoll = 0.f;
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
	void SetCameraToTagetDistance(float dist) { _distance = dist; }
	void SetCameraRotationYaw(float yaw) { _camYaw = yaw; }
	void SetCameraMinMaxRotationYaw(float min, float max);
	void SetCameraRotationPitch(float pitch) { _camPitch = pitch; }
	void SetCameraMinMaxRotationPitch(float min, float max);
	void SetCameraRotationRoll(float roll) { _camRoll = roll; }
	void SetCameraMinMaxRotationRoll(float min, float max);
	//Getter
	const Vec3& GetCameraPosition() const { return _camPos; }
	const Vec3& GetCameraLookVector() const { return _camlook; }
	const Vec3& GetCameraRightVector() const { return _camRight; }
	const Vec3& GetCameraUpVector() const { return _camUp; }
	const float& GetCameraToTargetDistance() const { return _distance; }
public:
	//Projection
	//Setter
	void SetNear(float value) { _near = value; }
	void SetPar(float value) { _far = value; }
	void SetFov(float value) { _fov = value; }
	void SetWidth(float value) { _width = value; }
	void SetHeight(float value) { _height = value; }
	void SetCameraType(CameraType type) { _camType = type; }
	void SetProjectionType(ProjectionType type);
	//Getter
	const ProjectionType& GetProjectionType() const { return _projType; }
	const Matrix& GetViewMatrix() const { return _matView; }
	const Matrix& GetProjectionMatrix() const { return _matProjection; }
private:
	void UpdateDefaultView();
	void UpdateTargetView();
	void UpdateMatrix();
	void UpdateVector();
public:
	void RotateAroundToTarget(const Vec3& pos, const Vec3& axis, float dist);
public:
	void Init(CameraType camType, ProjectionType projType, float dist);
	virtual void Update() override;
};

