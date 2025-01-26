#include "MenuUpdate.h"
#include "LevelUpdate.h"
#include "PlayerUpdate.h"
#include "SoundEngine.h"

MenuUpdate::MenuUpdate(RenderWindow* window)
{
	m_Window = window;
}

FloatRect* MenuUpdate::getPositionPointer()
{
	return &m_Position;
}

bool* MenuUpdate::getGameOverPointer()
{
	return &m_GameOver;
}

InputReceiver* MenuUpdate::getInputReceiver()
{
	return &m_InputReceiver;
}

void MenuUpdate::assemble(shared_ptr<LevelUpdate> levelUpdate, shared_ptr<PlayerUpdate> playerUpdate)
{
	m_PlayerPosition = playerUpdate->getPositionPointer();
	m_IsPaused = levelUpdate->getIsPausedPointer();

	m_Position.size.y = 75;
	m_Position.size.x = 75;

	SoundEngine::startMusic();
	SoundEngine::pauseMusic();
}

void MenuUpdate::handleInput()
{
	for(const Event& event: m_InputReceiver.getEvents())
	{
		if(const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
		{
			if(keyPressed->scancode == sf::Keyboard::Scancode::F1 && m_IsVisible)
			{
				if(SoundEngine::mMusicIsPlaying)
					SoundEngine::stopMusic();
				m_Window->close();
			}
		}

		if(const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
		{
			if(keyReleased->scancode == sf::Keyboard::Scancode::Escape)
			{
				m_IsVisible = !m_IsVisible;
				*m_IsPaused = !*m_IsPaused;

				if(m_GameOver)
					m_GameOver = false;

				if(!*m_IsPaused)
				{
					SoundEngine::resumeMusic();
					SoundEngine::playClick();
				}

				if(*m_IsPaused)
				{	
					SoundEngine::pauseMusic();
					SoundEngine::playClick();
				}
			}
		}
	}
	m_InputReceiver.clearEvents();
}

void MenuUpdate::update(float fps)
{
	handleInput();

	if(*m_IsPaused && !m_IsVisible) // Game over 1
	{
		m_IsVisible = true;
		m_GameOver = true;
	}

	if(m_IsVisible)
	{
		// Folow the player
		m_Position.position.x = m_PlayerPosition->position.x - m_Position.size.x / 2;
		m_Position.position.y =	m_PlayerPosition->position.y - m_Position.size.y / 2;
	}
	else
	{
		m_Position.position.x = -999;
		m_Position.position.y = -999;
	}
}
