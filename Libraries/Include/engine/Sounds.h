#pragma once
#include "fmod\fmod.h"
#include "fmod\fmod.hpp"
#include "fmod\fmod_errors.h"

class Transform;

class Sounds : public ResourceBase
{
	using Super = ResourceBase;

	//need each n files to create;
	FMOD::Channel* fm_Channel = nullptr;
	wstring _soundPath;
	FMOD::Sound* fm_Sound=nullptr;
	float volume=1;
	float volumeMultiply = 1;

public:
	Sounds();
	~Sounds();
public:
	void Load(const wstring& path);
	FMOD::Channel* Play(bool doLoop);
	FMOD::Channel* Play3D(bool doLoop, Vec3* pos, Vec3 vel = { 1,1,1 });
	void PlayEffect();
	void Play3DEffect(Vec3 *pos, Vec3 vel = { 1,1,1 });
	
	void SetVolume(float vol) {volume = vol; };
	float GetVolume() { return volume; };
	shared_ptr<Sounds> Clone();
	void StopChannel();
};