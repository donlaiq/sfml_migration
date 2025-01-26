#include "PlayerUpdate.h"
#include "SoundEngine.h"
#include "LevelUpdate.h"

FloatRect* PlayerUpdate::getPositionPointer()
{
	return &m_Position;
}

bool* PlayerUpdate::getGroundedPointer()
{
	return &m_IsGrounded;
}

InputReceiver* PlayerUpdate::getInputReceiver()
{
	return &m_InputReceiver;
}

void PlayerUpdate::assemble(shared_ptr<LevelUpdate> levelUpdate, shared_ptr<PlayerUpdate> playerUpdate)
{
	//SoundEngine::SoundEngine();

	m_Position.size.x = PLAYER_WIDTH;
	m_Position.size.y = PLAYER_HEIGHT;
	m_IsPaused = levelUpdate->getIsPausedPointer();
}

void PlayerUpdate::handleInput()
{
	for(const Event& event : m_InputReceiver.getEvents())
	{
		if(const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
		{
			if(keyPressed->scancode == sf::Keyboard::Scancode::D)
				m_RightIsHeldDown = true;
			if(keyPressed->scancode == sf::Keyboard::Scancode::A)
				m_LeftIsHeldDown = true;
			if(keyPressed->scancode == sf::Keyboard::Scancode::W)
				m_BoostIsHeldDown = true;
			if(keyPressed->scancode == sf::Keyboard::Scancode::Space)
				m_SpaceHeldDown = true;
		}

		if(const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
		{
			if(keyReleased->scancode == sf::Keyboard::Scancode::D)
				m_RightIsHeldDown = false;
			if(keyReleased->scancode == sf::Keyboard::Scancode::A)
				m_LeftIsHeldDown = false;
			if(keyReleased->scancode == sf::Keyboard::Scancode::W)
				m_BoostIsHeldDown = false;
			if(keyReleased->scancode == sf::Keyboard::Scancode::Space)
				m_SpaceHeldDown = false;
		}
	}

	m_InputReceiver.clearEvents();
}

void PlayerUpdate::update(float timeTakenThisFrame)
{
	if(!*m_IsPaused)
	{
		m_Position.position.y += m_Gravity * timeTakenThisFrame;

		handleInput();

		if(m_IsGrounded)
		{
			if(m_RightIsHeldDown)
				m_Position.position.x += timeTakenThisFrame * m_RunSpeed;
			if(m_LeftIsHeldDown)
				m_Position.position.x -= timeTakenThisFrame * m_RunSpeed;
		}

		if(m_BoostIsHeldDown)
		{
			m_Position.position.y -= timeTakenThisFrame * m_BoostSpeed;
			if(m_RightIsHeldDown)
				m_Position.position.x += timeTakenThisFrame * m_RunSpeed / 4;
			if(m_LeftIsHeldDown)
				m_Position.position.x -= timeTakenThisFrame	* m_RunSpeed / 4;
		}

		// Handle jumping
		if(m_SpaceHeldDown && !m_InJump && m_IsGrounded)
		{
			SoundEngine::playJump();
			m_InJump = true;
			m_JumpClock.restart();
		}

		if(!m_SpaceHeldDown)
		{
		}

		if(m_InJump)
		{
			if(m_JumpClock.getElapsedTime().asSeconds() < m_JumpDuration / 2)
				// Going up
				m_Position.position.y -= m_JumpSpeed * timeTakenThisFrame;
			else
				// Going down
				m_Position.position.y += m_JumpSpeed * timeTakenThisFrame;

			if(m_JumpClock.getElapsedTime().asSeconds() > m_JumpDuration)
				m_InJump = false;

			if(m_RightIsHeldDown)
				m_Position.position.x += timeTakenThisFrame * m_RunSpeed;

			if(m_LeftIsHeldDown)
				m_Position.position.x -= timeTakenThisFrame * m_RunSpeed;	
		}

		m_IsGrounded = false;
	}
}
