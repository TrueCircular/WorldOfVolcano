#pragma once

class CharacterController;

class HeightGetter : public MonoBehaviour
{
private:
	Terrain* _terrain;
	shared_ptr<CharacterController> _controller;
public: 
	float _height = 0.0f;
public:
	void Set(Terrain* terrain) { _terrain = terrain; }
	float GetHeight();
	float GetHeight(const Vec3& position);

	virtual void FixedUpdate();
	virtual void Update();
	virtual void LateUpdate();
};

