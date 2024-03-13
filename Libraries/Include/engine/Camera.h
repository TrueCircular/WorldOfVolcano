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
	Matrix _matView = Matrix::Identity;
	Matrix _matProjection = Matrix::Identity;
private:
	Vec3 _camPos = Vec3(0.f);
	Vec3 _camArounPos = Vec3(0,50,0);
	Vec3 _camOffset = Vec3(0.f);
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
	bool _camIsRotateAround = false;
	bool _camBackView = false;
private:
	shared_ptr<Transform>	_targetTransform = nullptr;
	Vec3					_targetPos = Vec3(0.f);
	Vec3					_targetLook = Vec3(0, 0, 1);
public:
	static Matrix S_MatView;
	static Matrix S_MatProjection;
public:
	//Camera
	//Setter
	void SetCameraPosition(const Vec3& pos) { _camPos = pos; }
	void SetCameraToTargetOffset(const Vec3& offset) { _camOffset = offset; }
	void SetTargetTransform(const shared_ptr<Transform>& target) { _targetTransform = target; }
	void SetCameraDistance(float dist) { _distance = dist; }
	void SetCameraRotationYaw(float yaw) { _camYaw = yaw; }
	void SetCameraMinMaxRotationYaw(float min, float max);
	void SetCameraRotationPitch(float pitch) { _camPitch = pitch; }
	void SetCameraMinMaxRotationPitch(float min, float max);
	void SetCameraRotationRoll(float roll) { _camRoll = roll; }
	void SetCameraMinMaxRotationRoll(float min, float max);
	void SetCameraIsRotationAround(bool flag) { _camIsRotateAround = flag; }
	//Getter
	const Vec3& GetCameraPosition() const { return _camPos; }
	const Vec3& GetCameraToTargetOffset() const { return _camOffset; }
	const shared_ptr<Transform>& GetCameraTargetTransform() const { return _targetTransform; }
	const float& GetCameraDistance() const { return _distance; }
	const float& GetCameraRotationYaw() const { return _camYaw; }
	std::tuple<float, float> GetCameraMinMaxRotationYaw() const { return std::make_tuple(_camMinYaw, _camMaxYaw); }
	const float& GetCameraRotationPitch() const { return _camYaw; }
	std::tuple<float, float> GetCameraMinMaxRotationPitch() const { return std::make_tuple(_camMinPitch, _camMaxPitch); }
	const float& GetCameraRotationRoll() const { return _camYaw; }
	std::tuple<float, float> GetCameraMinMaxRotationRoll() const { return std::make_tuple(_camMinRoll, _camMaxRoll); }
	const Vec3& GetCameraLookVector() const { return _camlook; }
	const Vec3& GetCameraRightVector() const { return _camRight; }
	const Vec3& GetCameraUpVector() const { return _camUp; }
	const bool& GetCameraIsRotationAround() const { return _camIsRotateAround; }
public:
	//Projection
	//Setter
	void SetNear(float value) { _near = value; }
	void SetFar(float value) { _far = value; }
	void SetFov(float value) { _fov = value; }
	void SetWidth(float value) { _width = value; }
	void SetHeight(float value) { _height = value; }
	void SetCameraType(CameraType type) { _camType = type; }
	void SetProjectionType(ProjectionType type);
	//Getter
	const float& GetNear() const { return _near; }
	const float& GetFar() const { return _far; }
	const float& GetFov() const { return _fov; }
	const float& GetWidth() const { return _width; }
	const float& GetHeight() const { return _height; }
	const CameraType& GetCameraType() const { return _camType; }
	const ProjectionType& GetProjectionType() const { return _projType; }
	const Matrix& GetViewMatrix() const { return _matView; }
	const Matrix& GetProjectionMatrix() const { return _matProjection; }
private:
	void UpdateDefaultView();
	void UpdateTargetView();
	void UpdateMatrix();
	void UpdateVector();
public:
	void RotateAroundToTarget(const Vec3& target, const Vec3& axis);
public:
	void Init(const Vec3& camPos, CameraType camType, ProjectionType projType, float distance);
	virtual void Update() override;
};

