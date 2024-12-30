#include "pch.h"
#include "SceneWarper.h"

void SceneWarper::Update()
{	
	if (playerTransform) {
		auto objTransform = GetGameObject()->GetTransform();
		Vec3 objPos = objTransform->GetLocalPosition();
		Vec3 playerPos = playerTransform->GetLocalPosition();

		objPos.y = 0;
		playerPos.y = 0;
		float dis = Vec3::Distance(objPos, playerPos);
		if (distance > dis)
		{
			if (isEnable != nullptr) {
				*isEnable = true;
			}
		}
		///NEED TO ADD OTHER SCRIPTS FOR DIVIDE
	}
}

SceneWarper::SceneWarper()
{
	_comName = "SceneWarp";
}

SceneWarper::~SceneWarper()
{
}
