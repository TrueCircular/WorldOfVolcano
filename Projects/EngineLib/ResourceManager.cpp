#include "pch.h"
#include "ResourceManager.h"
#include <filesystem>

ResourceManager* ResourceManager::_instance = nullptr;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

shared_ptr<Texture> ResourceManager::GetOrAddTexture(const wstring& key, const wstring& path)
{
	shared_ptr<Texture> texture = GetResource<Texture>(key);

	if (filesystem::exists(filesystem::path(path)) == false)
		return nullptr;

	texture = LoadResource<Texture>(key, path);

	if (texture == nullptr)
	{
		texture = make_shared<Texture>();
		texture->Load(path);
		AddResource(key, texture);
	}

	return texture;
}

void ResourceManager::CreateDefaultTexture()
{
	{
		shared_ptr<Texture> texture = make_shared<Texture>();
		texture->CreateTexture(L"..\\..\\Resources\\Texture\\NormalAlpha.png");
		AddResource(L"DefaultNormal", texture);

	}
	{
		shared_ptr<Texture> texture = make_shared<Texture>();
		texture->CreateTexture(L"..\\..\\Resources\\Texture\\SpecularNormal.png");
		AddResource(L"DefaultSpecular", texture);
	}
	{
		shared_ptr<Texture> texture = make_shared<Texture>();
		wstring dTex = RESOURCES_ADDR_TEXTURE;
		dTex += L"Effect/noise.png";
		texture->CreateTexture(dTex);
		AddResource(L"Dissolve", texture);
	}
}

void ResourceManager::CreateDefaultMesh()
{
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->CreateQuad();
		AddResource(L"Quad", mesh);
	} 
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->CreateQuadPlain();
		AddResource(L"QuadPlain", mesh);
	}
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->CreateCube();
		AddResource(L"Cube", mesh);
	}
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->CreateSphere();
		AddResource(L"Sphere", mesh);
	}
}

void ResourceManager::CreateDefaultShader()
{
	{
		auto Defaultshader = make_shared<Shader>(L"Instancing.fx");
		AddResource(L"Default", Defaultshader);
	}
}

void ResourceManager::CreateDefaultMaterial()
{
}

void ResourceManager::CreateDefaultAnimation()
{
}

void ResourceManager::Init()
{
	CreateDefaultTexture();
	CreateDefaultMesh();
	CreateDefaultShader();
}
