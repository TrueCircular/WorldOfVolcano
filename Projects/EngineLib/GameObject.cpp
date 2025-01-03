#include "pch.h"
#include "GameObject.h"
#include "BaseCollider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

GameObject::GameObject()
{
	_device = GRAPHICS()->GetDevice();
	_deviceContext = GRAPHICS()->GetDeviceContext();
}

GameObject::GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
{
	_device = device;
	_deviceContext = deviceContext;
}

GameObject::~GameObject()
{
}

shared_ptr<Component> GameObject::GetFixedComponent(ComponentType type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);

	return _components[index];
}

shared_ptr<Transform> GameObject::GetTransform()
{
	shared_ptr<Component> com = GetFixedComponent(ComponentType::Transform);

	return dynamic_pointer_cast<Transform>(com);
}

shared_ptr<Camera> GameObject::GetCamera()
{
	shared_ptr<Component> com = GetFixedComponent(ComponentType::Camera);

	return dynamic_pointer_cast<Camera>(com);
}

shared_ptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	shared_ptr<Component> com = GetFixedComponent(ComponentType::MeshRenderer);

	return dynamic_pointer_cast<MeshRenderer>(com);
}

shared_ptr<ModelRenderer> GameObject::GetModelRenderer()
{
	shared_ptr<Component> com = GetFixedComponent(ComponentType::ModelRenderer);

	return dynamic_pointer_cast<ModelRenderer>(com);
}

shared_ptr<ModelAnimator> GameObject::GetModelAnimator()
{
	shared_ptr<Component> com = GetFixedComponent(ComponentType::ModelAnimator);

	return dynamic_pointer_cast<ModelAnimator>(com);
}

std::shared_ptr<Transform> GameObject::GetOrAddTransform()
{
	if (GetTransform() == nullptr)
	{
		shared_ptr<Transform> transform = make_shared<Transform>();
		AddComponent(transform);
	}

	return GetTransform();
}

std::shared_ptr<Light> GameObject::GetLight()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Light);
	return static_pointer_cast<Light>(component);
}

shared_ptr<BaseCollider> GameObject::GetCollider()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Collider);

	return static_pointer_cast<BaseCollider>(component);
}

void GameObject::SetActive(bool active)
{
	if (_isActive == false )
	{
		if (active)
		{
			Start();
		}
	}

	_isActive = active;
}

void GameObject::SetName(wstring name)
{
	_name = name;
}

wstring GameObject::GetName()
{
	return _name;
}

void GameObject::SetParent(const shared_ptr<GameObject>& parent)
{
	_parent = parent;
}
void GameObject::AddChild(const shared_ptr<GameObject>& child)
{
	child->SetParent(shared_from_this());
	_children.push_back(child);

	GetTransform()->AddChild(child->GetTransform());
}

shared_ptr<GameObject> GameObject::GetChildByName(wstring name)
{
	shared_ptr<GameObject> temp;

	for (auto child : _children)
	{
		if (child->GetName() == name)
			temp = child;
	}

	return temp;
}

void GameObject::LoadGameObjcet(wstring& loadPath)
{

}

void GameObject::SaveGameObject(wstring& savePath)
{
	
}

void GameObject::AddComponent(shared_ptr<Component> component)
{
	component->SetGameObject(shared_from_this());

	uint8 index = static_cast<uint8>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT)
	{
		_components[index] = component;
	}
	else
	{
		_scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
}

void GameObject::Awake()
{
	if (GetTransform() == nullptr)
	{
		shared_ptr<Transform> trans = make_shared<Transform>();
		AddComponent(trans);
	}

	for (shared_ptr<Component>& com : _components)
	{
		if (com)
			com->Awake();
	}

	for (shared_ptr<MonoBehaviour>& com : _scripts)
	{
		com->Awake();
	}
}

void GameObject::Start()
{
	if (_isActive)
	{

		for (shared_ptr<Component>& com : _components)
		{
			if (com)
				com->Start();
		}

		for (shared_ptr<MonoBehaviour>& com : _scripts)
		{
			com->Start();
		}

	}
}

void GameObject::FixedUpdate()
{
	if (_isActive)
	{
		for (shared_ptr<Component>& com : _components)
		{
			if (com)
				com->FixedUpdate();
		}

		for (shared_ptr<MonoBehaviour>& com : _scripts)
		{
			com->FixedUpdate();
		}

		if (_children.size() > 0)
		{
			for (auto& ch : _children)
			{
				ch->FixedUpdate();
			}
		}
	}
}

void GameObject::Update()
{
	if (_isActive)
	{
		for (shared_ptr<Component>& com : _components)
		{
			if (com)
				com->Update();
		}

		for (shared_ptr<MonoBehaviour>& com : _scripts)
		{
			com->Update();
		}

		if (_children.size() > 0)
		{
			for (auto& ch : _children)
			{
				ch->Update();
			}
		}
	}
}

void GameObject::LateUpdate()
{
	if (_isActive)
	{
		for (shared_ptr<Component>& com : _components)
		{
			if (com)
				com->LateUpdate();
		}

		for (shared_ptr<MonoBehaviour>& com : _scripts)
		{
			com->LateUpdate();
		}

		if (_children.size() > 0)
		{
			for (auto& ch : _children)
			{
				ch->LateUpdate();
			}
		}
	}
}
