#pragma once

class SpringArm : public MonoBehaviour
{
public:
	SpringArm();
	virtual ~SpringArm();
private:
	shared_ptr<Camera> _camera;
	Vec3 _position;
public:
	void SetCamera(const shared_ptr<Camera> cam) { _camera = cam; }
public:
	virtual void Update() override;
};

