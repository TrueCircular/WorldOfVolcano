#include "pch.h"
#include "Ragnaros.h"

Ragnaros::Ragnaros()
{
	_objType = ObjectType::EnemyUnit;
}

Ragnaros::~Ragnaros()
{
}

void Ragnaros::CharacterInit()
{
}

void Ragnaros::Awake()
{
	Super::Awake();
	CharacterInit();
}

void Ragnaros::Start()
{
	Super::Start();
}

void Ragnaros::FixedUpdate()
{
	Super::FixedUpdate();
}

void Ragnaros::Update()
{
	Super::Update();
}

void Ragnaros::LateUpdate()
{
	Super::LateUpdate();
}
