#include "FireballUpdate.h"
#include <random>
#include "SoundEngine.h"
#include "FireballUpdate.h"
#include "PlayerUpdate.h"

FireballUpdate::FireballUpdate(bool* pausedPointer)
{
	m_GameIsPaused = pausedPointer;
	m_PauseDurationTarget = getRandomNumber(m_MinPause, m_MaxPause);
}

bool* FireballUpdate::getFacingRightPointer()
{
	return &m_LeftToRight;
}

FloatRect* FireballUpdate::getPositionPointer()
{
	return &m_Position;
}

void FireballUpdate::assemble(shared_ptr<LevelUpdate> levelUpdate, shared_ptr<PlayerUpdate> playerUpdate)
{
	m_PlayerPosition = playerUpdate->getPositionPointer();
	m_Position.position.y = getRandomNumber(m_PlayerPosition->position.y - m_MaxSpawnDistanceFromPlayer, m_PlayerPosition->position.y + m_MaxSpawnDistanceFromPlayer);
	m_Position.position.x = m_PlayerPosition->position.x - getRandomNumber(200, 400);

	m_Position.size.x = 10;
	m_Position.size.y = 10;
}

int FireballUpdate::getRandomNumber(int minHeight, int maxHeight)
{
	// Seed the random number generator with current time
	std::random_device rd;
	std::mt19937 gen(rd());

	// Define a uniform distribution for the desired range
	std::uniform_int_distribution<int> distribution(minHeight, maxHeight);

	// Generate a random height within the specified range
	int randomHeight = distribution(gen);

	return randomHeight;
}

void FireballUpdate::update(float fps)
{
	if(!*m_GameIsPaused)
	{
		if(!m_MovementPaused)
		{
			if(m_LeftToRight)
			{
				m_Position.position.x += m_Speed * fps;
				if(m_Position.position.x - m_PlayerPosition->position.x > m_Range)
				{
					m_MovementPaused = true;
					m_PauseClock.restart();
					m_LeftToRight = !m_LeftToRight;
					m_Position.position.y = getRandomNumber(m_PlayerPosition->position.y - m_MaxSpawnDistanceFromPlayer, m_PlayerPosition->position.y + m_MaxSpawnDistanceFromPlayer);
					m_PauseDurationTarget = getRandomNumber(m_MinPause, m_MaxPause);
				}
			}
			else
			{
			
				m_Position.position.x -= m_Speed * fps;
				if(m_PlayerPosition->position.x - m_Position.position.x > m_Range)
				{
					m_MovementPaused = true;
					m_PauseClock.restart();
					m_LeftToRight = !m_LeftToRight;
					m_Position.position.y = getRandomNumber(m_PlayerPosition->position.y - m_MaxSpawnDistanceFromPlayer, m_PlayerPosition->position.y + m_MaxSpawnDistanceFromPlayer);
					m_PauseDurationTarget = getRandomNumber(m_MinPause, m_MaxPause);
				}
			}

			// Has it hit the player
			if(m_PlayerPosition->findIntersection(m_Position))
			{
				// Knock the player down
				m_PlayerPosition->position.y = m_PlayerPosition->position.y + m_PlayerPosition->size.y * 2;
			}
		}
		else
		{
			//if(m_PauseClock.getElapsedTime().asSeconds() > m_PauseDurationTarget)
			{
				m_MovementPaused = false;
				SoundEngine::playFireballLaunch(m_PlayerPosition->position, m_Position.position);
			}
		}
	}
}
