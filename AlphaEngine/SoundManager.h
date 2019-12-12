#pragma once
#include "Singleton.h"
#include <fmod.hpp>
#include <vector>

enum SoundAssets
{
	BG_Stage,
	FX_StageStart,
	FX_Move,
	FX_Acquire,
	FX_Clear,
	SoundAssetNum
};

enum SoundChannel
{
	Background,
	FX
};

static const char* SoundAssetPath[SoundAssetNum] = {
	"../Assets/Sound/bg.mp3",
	"../Assets/Sound/clear.mp3",
	"../Assets/Sound/move.wav",
	"../Assets/Sound/object.wav",
	"../Assets/Sound/clear.mp3",
};

class SoundManager : public Singleton<SoundManager> 
{
	FMOD::System* _system;
	FMOD::Channel* _channel[2];
	std::vector<FMOD::Sound*> _sounds;

	SoundChannel GetChannel(SoundAssets sound);
	
public:
	SoundManager();
	virtual ~SoundManager();

	void Init();
	void Update();
	void Release();

	void PlaySound(SoundAssets sound);
	void StopSound(SoundAssets sound);
	bool IsPlaying(SoundAssets sound);
	bool IsPlaying(SoundChannel channel);
	void StopChannel(SoundChannel channel);
	void PauseChannel(SoundChannel channel);
	void ResumeChannel(SoundChannel channel);
};

