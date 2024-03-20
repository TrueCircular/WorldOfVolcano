#include "pch.h"
#include "Unit.h"
#include "BaseCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "CharacterController.h"
#include "PlayerController.h"
#include "AIController.h"

void Unit::AddAnimation(const shared_ptr<Model>& com, wstring animOwner, wstring animName)
{
	wstring addr = RESOURCES_ADDR_ANIMATION;
	addr += animOwner;
	addr += L"/";
	addr += animName;
	addr += L".anim";

	com->ReadAnimation(addr);
}

void Unit::AddModelAndMaterial(const shared_ptr<Model>& com, wstring name)
{
	wstring MeshAdr = RESOURCES_ADDR_MESH_SKELETAL;
	MeshAdr += name;
	MeshAdr += L"/";
	MeshAdr += name;
	MeshAdr += L".mesh";

	wstring MaterialAdr = RESOURCES_ADDR_TEXTURE_SKELETAL;
	MaterialAdr += name;
	MaterialAdr += L"/";
	MaterialAdr += name;
	MaterialAdr += L".xml";

	com->ReadModel(MeshAdr);
	com->ReadMaterial(MaterialAdr);
}

void Unit::SetSpwanPosition(const Vec3& spwanPos)
{
	auto playerCon = GetComponent<PlayerController>();
	auto aiCon = GetComponent<AIController>();

	if (playerCon != nullptr)
	{
		playerCon->SetSpwanPosition(spwanPos);
		GetTransform()->SetLocalPosition(spwanPos);
	}
	else if (aiCon != nullptr)
	{
		aiCon->SetSpwanPosition(spwanPos);
		GetTransform()->SetLocalPosition(spwanPos);
	}
}

void Unit::SetCharacterController(const shared_ptr<CharacterController>& controller, AIType aiType)
{
	auto playerCon = dynamic_pointer_cast<PlayerController>(controller);
	auto aiCon = dynamic_pointer_cast<AIController>(controller);

	if (playerCon != nullptr)
	{
		AddComponent(playerCon);
	}
	else if (aiCon != nullptr)
	{
		aiCon->SetAIType(aiType);
		AddComponent(aiCon);
	}
}

void Unit::Awake()
{
	Super::Awake();
}

void Unit::Start()
{
	Super::Start();
}

void Unit::FixedUpdate()
{
	Super::FixedUpdate();
}

void Unit::Update()
{
	Super::Update();
}

void Unit::LateUpdate()
{
	Super::LateUpdate();
}
