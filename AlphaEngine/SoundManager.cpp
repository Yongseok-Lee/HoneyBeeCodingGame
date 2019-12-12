#include "SoundManager.h"
#include <cassert>

SoundChannel SoundManager::GetChannel(SoundAssets sound)
{
	SoundChannel channel;
	switch (sound)
	{
	case BG_Stage:
		channel = Background; break;
	default: channel = FX;
	}
	return channel;
}

SoundManager::SoundManager() : Singleton<SoundManager>()
{
	assert(FMOD::System_Create(&_system) == FMOD_OK);
}

SoundManager::~SoundManager()
{
}

void SoundManager::Init()
{
	assert(_system->init(2, FMOD_INIT_NORMAL, nullptr) == FMOD_OK);
	_sounds.resize(SoundAssetNum);
	for (int i = 0; i < SoundAssetNum; ++i)
	{
		_system->createSound(SoundAssetPath[i], FMOD_DEFAULT, nullptr, &_sounds[i]);
	}
}

void SoundManager::Update()
{
	_system->update();
}

void SoundManager::Release()
{
	for (int i = 0; i < SoundAssetNum; ++i)
	{
		_sounds[i]->release();
	}
	
	_system->release();
}

void SoundManager::PlaySound(SoundAssets sound)
{
	_system->playSound(_sounds[sound], nullptr, false, &_channel[GetChannel(sound)]);
}

void SoundManager::StopSound(SoundAssets sound)
{
	_channel[GetChannel(sound)]->stop();
}

bool SoundManager::IsPlaying(SoundAssets sound)
{
	bool result;
	_channel[GetChannel(sound)]->isPlaying(&result);
	return result;
}

bool SoundManager::IsPlaying(SoundChannel channel)
{
	bool result;
	_channel[channel]->isPlaying(&result);
	return result;
}

void SoundManager::StopChannel(SoundChannel channel)
{
	_channel[channel]->stop();
}

void SoundManager::PauseChannel(SoundChannel channel)
{
	_channel[channel]->setPaused(true);
}

void SoundManager::ResumeChannel(SoundChannel channel)
{
	_channel[channel]->setPaused(false);
}
