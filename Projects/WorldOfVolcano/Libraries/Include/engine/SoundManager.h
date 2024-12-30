#pragma once
#include "Sounds.h"
#include <list>


class SoundManager
{
	struct SoundRef  {
		Vec3* pos;
		float soundVol;
	};
//	list<pair<wstring,FMOD::Channel*>> channelList;
	shared_ptr<Transform> soundTransform;
	FMOD_VECTOR prevPos = {1,1,1};
	vector <pair<SoundRef, FMOD::Channel*>> managed_channels;
	vector <FMOD::Channel*> managed_2Dchannels;
	static SoundManager* _instance;
	float soundVolume=1;
private:
	SoundManager() {

	}
public:
	static SoundManager* GetInstance();
	~SoundManager();
public:
	void Init();
	void Update();
	void SetTransForm(shared_ptr<Transform> refTransform) {
		soundTransform = refTransform;
	}
	void AddChannel(float refvol,Vec3* pos,FMOD::Channel* channel) {
		SoundRef ref;
		ref.soundVol = refvol;
		ref.pos = pos;
		managed_channels.push_back(make_pair(ref,channel));
	}
	void AddChannel(FMOD::Channel* channel) {
		managed_2Dchannels.push_back(channel);
	}
	void SetVolume(float volume) {
		soundVolume = volume;
		if (soundVolume > 1) {
			soundVolume = 1;
		}
		if (g_fmSystem)
		{
			for (auto c : managed_2Dchannels) {
				c->setVolume(volume);
			}
			for (auto c : managed_channels) {
				float vol;
				c.second->getVolume(&vol);
				vol = (vol * soundVolume);
				c.second->setVolume(vol);
			}
		}
	}
	float GetCurrentVolume() { return soundVolume; }
//	void Pause();
//	void Resume();
//	void Stop();
};

