#pragma once

class Scene;

class SceneWarper :public MonoBehaviour
{
	bool* isEnable = nullptr;
	shared_ptr<Scene> targetScene=nullptr;
	shared_ptr<Transform> playerTransform;
	float distance = 10.0f;
public:
	void SetFlag(bool* flag) { isEnable = flag; };
	void SetScene(shared_ptr<Scene> scene) { targetScene = scene; };

public: 
	virtual void Update() override;
	SceneWarper();
	~SceneWarper();
};

