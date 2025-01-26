#include "PlatformUpdate.h"
#include "PlayerUpdate.h"

FloatRect* PlatformUpdate::getPositionPointer()
{
	return &m_Position;
}

void PlatformUpdate::assemble(shared_ptr<LevelUpdate> levelUpdate, shared_ptr<PlayerUpdate> playerUpdate)
{
	m_PlayerPosition = playerUpdate->getPositionPointer();
	m_PlayerIsGrounded = playerUpdate->getGroundedPointer();
}

void PlatformUpdate::update(float fps)
{
	if(m_Position.findIntersection(*m_PlayerPosition).has_value())
	{
		Vector2f playerFeet(m_PlayerPosition->position.x + m_PlayerPosition->size.x / 2, m_PlayerPosition->position.y + m_PlayerPosition->size.y);
		Vector2f playerRight(m_PlayerPosition->position.x + m_PlayerPosition->size.x, m_PlayerPosition->position.y + m_PlayerPosition->size.y / 2);		
		Vector2f playerLeft(m_PlayerPosition->position.x, m_PlayerPosition->position.y + m_PlayerPosition->size.y / 2);
		Vector2f playerHead(m_PlayerPosition->position.x + m_PlayerPosition->size.x / 2, m_PlayerPosition->position.y);

		if(m_Position.contains(playerFeet))
		{
			if(playerFeet.y > m_Position.position.y)
			{
				m_PlayerPosition->position.y = m_Position.position.y - m_PlayerPosition->size.y;
				*m_PlayerIsGrounded = true;
			}
		}
		else if(m_Position.contains(playerRight))
			m_PlayerPosition->position.x = m_Position.position.x - m_PlayerPosition->size.x;
		else if(m_Position.contains(playerLeft))
			m_PlayerPosition->position.x = m_Position.position.x + m_Position.size.x;
		else if(m_Position.contains(playerHead))
			m_PlayerPosition->position.y = m_Position.position.y + m_Position.size.y;
	}
}
