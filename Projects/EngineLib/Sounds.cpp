#include "pch.h"
#include "Sounds.h"

FMOD::System* g_fmSystem;

Sounds::Sounds() :Super(ResourceType::Sounds)
{
}

Sounds::~Sounds()
{	
	if (fm_Sound)fm_Sound->release();
}

void Sounds::Load(const wstring& path)
{
	_soundPath = wstring(path);

	if (g_fmSystem == nullptr) {
		FMOD::System_Create(&g_fmSystem);
		g_fmSystem->init(128, FMOD_INIT_NORMAL, 0);

	}
	string multipath;
	multipath.assign(path.begin(),path.end());
	FMOD_RESULT result = g_fmSystem->createSound(multipath.c_str(), FMOD_DEFAULT, nullptr, &fm_Sound);
	if (result != FMOD_OK) {
		assert("ERROR FROM SOUNDCLASS FMOD");
	}
}
FMOD::Channel* Sounds::Play(bool doLoop)
{
	FMOD::Channel* channel;

	FMOD_RESULT fr;
	(doLoop) ? fr = fm_Sound->setMode(FMOD_LOOP_NORMAL) : fr = fm_Sound->setMode(FMOD_LOOP_OFF);
	fr=fm_Sound->setMode(FMOD_2D);
	fr=g_fmSystem->playSound(fm_Sound, nullptr, false, &channel);
	fm_Channel = channel;
	float currvol = (volume * MANAGER_SOUND()->GetCurrentVolume());
	channel->setVolume(currvol);
	MANAGER_SOUND()->AddChannel(channel);
	return channel;
}
FMOD::Channel* Sounds::Play3D(bool doLoop, Vec3 *pos, Vec3 vel)
{
	FMOD::Channel* channel;

	FMOD_VECTOR position;
	FMOD_VECTOR velocity;
	position.x = pos->x;
	position.y = pos->y;
	position.z = pos->z;
	velocity.x = vel.x;
	velocity.y = vel.y;
	velocity.z = vel.z;
	
	FMOD_RESULT fr;
	(doLoop) ? fr=fm_Sound->setMode(FMOD_LOOP_NORMAL) : fr=fm_Sound->setMode(FMOD_LOOP_OFF);
	fr=fm_Sound->setMode(FMOD_3D);
	fr=g_fmSystem->playSound(fm_Sound, nullptr, false, &channel);
	fm_Channel = channel;
//	fm_Sound->get3DCustomRolloff(&vec,&ids);
	float currvol = (volume * MANAGER_SOUND()->GetCurrentVolume());
	channel->setVolume(currvol);
	fr=channel->set3DAttributes(&position, &velocity);
	channel->set3DMinMaxDistance(10,100);

	MANAGER_SOUND()->AddChannel(volume,pos,channel);
//	c
	return channel;
}
void Sounds::PlayEffect()
{
	FMOD::Channel* channel;
	fm_Sound->setMode(FMOD_2D);
	g_fmSystem->playSound(fm_Sound, nullptr, false, &channel);
	fm_Channel = channel;
	float currvol = (volume * MANAGER_SOUND()->GetCurrentVolume());
	channel->setVolume(currvol);
	MANAGER_SOUND()->AddChannel(channel);

}
void Sounds::Play3DEffect(Vec3* pos,Vec3 vel )
{
	FMOD::Channel* channel;

	FMOD_VECTOR position;
	FMOD_VECTOR velocity;
	position.x = pos->x;
	position.y = pos->y;
	position.z = pos->z;
	velocity.x = vel.x;
	velocity.y = vel.y;
	velocity.z = vel.z;
	fm_Sound->setMode(FMOD_3D);

	g_fmSystem->playSound(fm_Sound, nullptr, false, &channel);
	fm_Channel = channel;
	channel->set3DAttributes(&position, &velocity);
	float currvol = (volume * MANAGER_SOUND()->GetCurrentVolume());
	channel->setVolume(currvol);
	MANAGER_SOUND()->AddChannel(volume, pos, channel);
}

shared_ptr<Sounds> Sounds::Clone()
{
	if (_soundPath == L"")
		return nullptr;

	shared_ptr<Sounds> newSound = make_shared<Sounds>();

	{
		if (g_fmSystem == nullptr) {
			FMOD::System_Create(&g_fmSystem);
			g_fmSystem->init(128, FMOD_INIT_NORMAL, 0);

		}

		string multipath;

		multipath.assign(_soundPath.begin(), _soundPath.end());
		FMOD_RESULT result = g_fmSystem->createSound(multipath.c_str(), FMOD_DEFAULT, nullptr, &newSound->fm_Sound);
		if (result != FMOD_OK) {
			assert("ERROR FROM SOUNDCLASS FMOD");
		}
	}

	newSound->_soundPath = _soundPath;
	newSound->volume = volume;
	return newSound;
}

void Sounds::StopChannel()
{
	if (fm_Channel != nullptr) {
		fm_Channel->stop();
		fm_Channel = nullptr;
	}
}

//(Vec3 pos, Vec3 up, Vec3 forward, Vec3 velocity = { 1,1,1 })
//{
//	FMOD_3D_ATTRIBUTES _3DsoundDesc;
//	_3DsoundDesc.position.x = pos.x;
//	_3DsoundDesc.position.y = pos.y;
//	_3DsoundDesc.position.z = pos.z;
//	_3DsoundDesc.up.x = up.x;
//	_3DsoundDesc.up.y = up.y;
//	_3DsoundDesc.up.z = up.z;
//	_3DsoundDesc.forward.x = forward.x;
//	_3DsoundDesc.forward.y = forward.y;
//	_3DsoundDesc.forward.z = forward.z;
//	_3DsoundDesc.velocity.x = velocity.x;
//	_3DsoundDesc.velocity.y = velocity.y;
//	_3DsoundDesc.velocity.z = velocity.z;
