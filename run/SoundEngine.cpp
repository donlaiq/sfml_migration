#include "SoundEngine.h"
#include <assert.h>

SoundEngine* SoundEngine::m_s_Instance = nullptr;
bool SoundEngine::mMusicIsPlaying = false;

Music SoundEngine::music;
SoundBuffer SoundEngine::m_ClickBuffer{"sound/click.wav"};
Sound SoundEngine::m_ClickSound{m_ClickBuffer};
SoundBuffer SoundEngine::m_JumpBuffer{"sound/jump.wav"};
Sound SoundEngine::m_JumpSound{m_JumpBuffer};
SoundBuffer SoundEngine::mFireballLaunchBuffer{"sound/fireballLaunch.wav"};
Sound SoundEngine::mFireballLaunchSound{mFireballLaunchBuffer};

SoundEngine::SoundEngine()
{
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;

	Listener::setDirection({1.f, 0.f, 0.f});
	Listener::setUpVector({1.f, 1.f, 0.f});
	Listener::setGlobalVolume(100.f);
}

void SoundEngine::playClick()
{
	m_ClickSound.play();
}

void SoundEngine::playJump()
{
	m_JumpSound.play();
}

void SoundEngine::startMusic()
{
	music.openFromFile("music/music.wav");
	m_s_Instance->music.play();
	m_s_Instance->music.setLooping(true);
	mMusicIsPlaying = true;
}

void SoundEngine::pauseMusic()
{
	m_s_Instance->music.pause();
	mMusicIsPlaying = false;
}

void SoundEngine::resumeMusic()
{
	m_s_Instance->music.play();
	mMusicIsPlaying = true;
}

void SoundEngine::stopMusic()
{
	m_s_Instance->music.stop();
	mMusicIsPlaying = false;
}

void SoundEngine::playFireballLaunch(Vector2f playerPosition, Vector2f soundLocation)
{
	mFireballLaunchSound.setRelativeToListener(true);

	if(playerPosition.x > soundLocation.x)
	// coming from the left
	{
		Listener::setPosition({0, 0, 0.f});
		mFireballLaunchSound.setPosition({-100, 0, 0.f});
		mFireballLaunchSound.setMinDistance(100);
		mFireballLaunchSound.setAttenuation(0);
	}
	else
	// coming from the right
	{
		Listener::setPosition({0, 0, 0.f});
		mFireballLaunchSound.setPosition({100, 0, 0.f});
		mFireballLaunchSound.setMinDistance(100);
		mFireballLaunchSound.setAttenuation(0);
	}
	mFireballLaunchSound.play();
}
