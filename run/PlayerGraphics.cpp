#include "PlayerGraphics.h"
#include "PlayerUpdate.h"
#include "Animator.h"

void PlayerGraphics::assemble(VertexArray& canvas, shared_ptr<Update> genericUpdate, IntRect texCoords)
{
	m_PlayerUpdate = static_pointer_cast<PlayerUpdate>(genericUpdate);
	m_Position = m_PlayerUpdate->getPositionPointer();

	m_Animator = new Animator(texCoords.position.x, texCoords.position.y, 6, texCoords.size.x * 6, texCoords.size.y, 12);

	// Get the first frame of animation
	m_SectionToDraw = m_Animator->getCurrentFrame(false);
	m_StandingStillSectionToDraw = m_Animator->getCurrentFrame(false);

	m_VertexStartIndex = canvas.getVertexCount();
	canvas.resize(canvas.getVertexCount() + 6);
}

void PlayerGraphics::draw(VertexArray& canvas)
{
	const Vector2f& position = m_Position->position;
	const Vector2f& scale = m_Position->size;

	canvas[m_VertexStartIndex].position = position;
	canvas[m_VertexStartIndex + 1].position = position + Vector2f(scale.x, 0);
	canvas[m_VertexStartIndex + 2].position = position + scale;
	canvas[m_VertexStartIndex + 3].position = position + scale;
	canvas[m_VertexStartIndex + 4].position = position;
	canvas[m_VertexStartIndex + 5].position = position + Vector2f(0, scale.y);

	if(m_PlayerUpdate->m_RightIsHeldDown && !m_PlayerUpdate->m_InJump && !m_PlayerUpdate->m_BoostIsHeldDown && m_PlayerUpdate->m_IsGrounded)
		m_SectionToDraw = m_Animator->getCurrentFrame(false);	
	if(m_PlayerUpdate->m_LeftIsHeldDown && !m_PlayerUpdate->m_InJump && !m_PlayerUpdate->m_BoostIsHeldDown && m_PlayerUpdate->m_IsGrounded)
		m_SectionToDraw = m_Animator->getCurrentFrame(true);
	else
	{
		// Test the players	facing position in case it changed while jumping or boosting 
		// This value is used in the final animation option
		if(m_PlayerUpdate->m_LeftIsHeldDown)
			m_LastFacingRight = false;
		else
			m_LastFacingRight = true;
	}

	const int uPos = m_SectionToDraw->position.x;
	const int vPos = m_SectionToDraw->position.y;
	const int texWidth = m_SectionToDraw->size.x;
	const int texHeight = m_SectionToDraw->size.y;

	if(m_PlayerUpdate->m_RightIsHeldDown && !m_PlayerUpdate->m_InJump && !m_PlayerUpdate->m_BoostIsHeldDown)
	{
		canvas[m_VertexStartIndex].texCoords.x = uPos;
		canvas[m_VertexStartIndex].texCoords.y = vPos;
		canvas[m_VertexStartIndex + 1].texCoords.x = uPos + texWidth;
		canvas[m_VertexStartIndex + 1].texCoords.y = vPos;
		canvas[m_VertexStartIndex + 2].texCoords.x = uPos + texWidth;
		canvas[m_VertexStartIndex + 2].texCoords.y = vPos + texHeight;
		canvas[m_VertexStartIndex + 3].texCoords.x = uPos + texWidth;
		canvas[m_VertexStartIndex + 3].texCoords.y = vPos + texHeight;
		canvas[m_VertexStartIndex + 4].texCoords.x = uPos;
		canvas[m_VertexStartIndex + 4].texCoords.y = vPos;
		canvas[m_VertexStartIndex + 5].texCoords.x = uPos;
		canvas[m_VertexStartIndex + 5].texCoords.y = vPos + texHeight;
	}
	else if(m_PlayerUpdate->m_LeftIsHeldDown && !m_PlayerUpdate->m_InJump && !m_PlayerUpdate->m_BoostIsHeldDown)
	{
		canvas[m_VertexStartIndex].texCoords.x = uPos;
		canvas[m_VertexStartIndex].texCoords.y = vPos;
		canvas[m_VertexStartIndex + 1].texCoords.x = uPos - texWidth;
		canvas[m_VertexStartIndex + 1].texCoords.y = vPos;
		canvas[m_VertexStartIndex + 2].texCoords.x = uPos - texWidth;
		canvas[m_VertexStartIndex + 2].texCoords.y = vPos + texHeight;
		canvas[m_VertexStartIndex + 3].texCoords.x = uPos - texWidth;
		canvas[m_VertexStartIndex + 3].texCoords.y = vPos + texHeight;
		canvas[m_VertexStartIndex + 4].texCoords.x = uPos;
		canvas[m_VertexStartIndex + 4].texCoords.y = vPos;
		canvas[m_VertexStartIndex + 5].texCoords.x = uPos;
		canvas[m_VertexStartIndex + 5].texCoords.y = vPos + texHeight;
	}
	else if(m_PlayerUpdate->m_RightIsHeldDown && m_PlayerUpdate->m_BoostIsHeldDown)
	{
		canvas[m_VertexStartIndex].texCoords.x = BOOST_TEX_LEFT;
		canvas[m_VertexStartIndex].texCoords.y = BOOST_TEX_TOP;
		canvas[m_VertexStartIndex + 1].texCoords.x = BOOST_TEX_LEFT + BOOST_TEX_WIDTH;
		canvas[m_VertexStartIndex + 1].texCoords.y = BOOST_TEX_TOP;
		canvas[m_VertexStartIndex + 2].texCoords.x = BOOST_TEX_LEFT + BOOST_TEX_WIDTH;
		canvas[m_VertexStartIndex + 2].texCoords.y = BOOST_TEX_TOP + BOOST_TEX_HEIGHT;
		canvas[m_VertexStartIndex + 3].texCoords.x = BOOST_TEX_LEFT + BOOST_TEX_WIDTH;
		canvas[m_VertexStartIndex + 3].texCoords.y = BOOST_TEX_TOP + BOOST_TEX_HEIGHT;
		canvas[m_VertexStartIndex + 4].texCoords.x = BOOST_TEX_LEFT;
		canvas[m_VertexStartIndex + 4].texCoords.y = BOOST_TEX_TOP;
		canvas[m_VertexStartIndex + 5].texCoords.x = BOOST_TEX_LEFT;
		canvas[m_VertexStartIndex + 5].texCoords.y = BOOST_TEX_TOP + BOOST_TEX_HEIGHT;
	}
	else if(m_PlayerUpdate->m_LeftIsHeldDown && m_PlayerUpdate->m_BoostIsHeldDown)
	{
		canvas[m_VertexStartIndex].texCoords.x = BOOST_TEX_LEFT + BOOST_TEX_WIDTH;
		canvas[m_VertexStartIndex].texCoords.y = BOOST_TEX_TOP;
		canvas[m_VertexStartIndex + 1].texCoords.x = BOOST_TEX_LEFT;
		canvas[m_VertexStartIndex + 1].texCoords.y = BOOST_TEX_TOP;
		canvas[m_VertexStartIndex + 2].texCoords.x = BOOST_TEX_LEFT;
		canvas[m_VertexStartIndex + 2].texCoords.y = BOOST_TEX_TOP + BOOST_TEX_HEIGHT;
		canvas[m_VertexStartIndex + 3].texCoords.x = BOOST_TEX_LEFT;
		canvas[m_VertexStartIndex + 3].texCoords.y = BOOST_TEX_TOP + BOOST_TEX_HEIGHT;
		canvas[m_VertexStartIndex + 4].texCoords.x = BOOST_TEX_LEFT + BOOST_TEX_WIDTH;
		canvas[m_VertexStartIndex + 4].texCoords.y = BOOST_TEX_TOP;
		canvas[m_VertexStartIndex + 5].texCoords.x = BOOST_TEX_LEFT + BOOST_TEX_WIDTH;
		canvas[m_VertexStartIndex + 5].texCoords.y = BOOST_TEX_TOP + BOOST_TEX_HEIGHT;
	}
	else if(m_PlayerUpdate->m_BoostIsHeldDown)
	{
		canvas[m_VertexStartIndex].texCoords.x = BOOST_TEX_LEFT;
		canvas[m_VertexStartIndex].texCoords.y = BOOST_TEX_TOP;
		canvas[m_VertexStartIndex + 1].texCoords.x = BOOST_TEX_LEFT + BOOST_TEX_WIDTH;
		canvas[m_VertexStartIndex + 1].texCoords.y = BOOST_TEX_TOP;
		canvas[m_VertexStartIndex + 2].texCoords.x = BOOST_TEX_LEFT + BOOST_TEX_WIDTH;
		canvas[m_VertexStartIndex + 2].texCoords.y = BOOST_TEX_TOP + BOOST_TEX_HEIGHT;
		canvas[m_VertexStartIndex + 3].texCoords.x = BOOST_TEX_LEFT + BOOST_TEX_WIDTH;
		canvas[m_VertexStartIndex + 3].texCoords.y = BOOST_TEX_TOP + BOOST_TEX_HEIGHT;
		canvas[m_VertexStartIndex + 4].texCoords.x = BOOST_TEX_LEFT;
		canvas[m_VertexStartIndex + 4].texCoords.y = BOOST_TEX_TOP;
		canvas[m_VertexStartIndex + 5].texCoords.x = BOOST_TEX_LEFT;
		canvas[m_VertexStartIndex + 5].texCoords.y = BOOST_TEX_TOP + BOOST_TEX_HEIGHT;
	}
	else
	{
		if(m_LastFacingRight)
		{	
			canvas[m_VertexStartIndex].texCoords.x = m_StandingStillSectionToDraw->position.x;
			canvas[m_VertexStartIndex].texCoords.y = m_StandingStillSectionToDraw->position.y;
			canvas[m_VertexStartIndex + 1].texCoords.x = m_StandingStillSectionToDraw->position.x + texWidth;
			canvas[m_VertexStartIndex + 1].texCoords.y = m_StandingStillSectionToDraw->position.y;
			canvas[m_VertexStartIndex + 2].texCoords.x = m_StandingStillSectionToDraw->position.x + texWidth;
			canvas[m_VertexStartIndex + 2].texCoords.y = m_StandingStillSectionToDraw->position.y + texHeight;
			canvas[m_VertexStartIndex + 3].texCoords.x = m_StandingStillSectionToDraw->position.x + texWidth;
			canvas[m_VertexStartIndex + 3].texCoords.y = m_StandingStillSectionToDraw->position.y + texHeight;
			canvas[m_VertexStartIndex + 4].texCoords.x = m_StandingStillSectionToDraw->position.x;
			canvas[m_VertexStartIndex + 4].texCoords.y = m_StandingStillSectionToDraw->position.y;
			canvas[m_VertexStartIndex + 5].texCoords.x = m_StandingStillSectionToDraw->position.x;
			canvas[m_VertexStartIndex + 5].texCoords.y = m_StandingStillSectionToDraw->position.y + texHeight;
		}
		else
		{
			
			canvas[m_VertexStartIndex].texCoords.x = m_StandingStillSectionToDraw->position.x + texWidth;
			canvas[m_VertexStartIndex].texCoords.y = m_StandingStillSectionToDraw->position.y;
			canvas[m_VertexStartIndex + 1].texCoords.x = m_StandingStillSectionToDraw->position.x;
			canvas[m_VertexStartIndex + 1].texCoords.y = m_StandingStillSectionToDraw->position.y;
			canvas[m_VertexStartIndex + 2].texCoords.x = m_StandingStillSectionToDraw->position.x;
			canvas[m_VertexStartIndex + 2].texCoords.y = m_StandingStillSectionToDraw->position.y + texHeight;
			canvas[m_VertexStartIndex + 3].texCoords.x = m_StandingStillSectionToDraw->position.x;
			canvas[m_VertexStartIndex + 3].texCoords.y = m_StandingStillSectionToDraw->position.y + texHeight;
			canvas[m_VertexStartIndex + 4].texCoords.x = m_StandingStillSectionToDraw->position.x + texWidth;
			canvas[m_VertexStartIndex + 4].texCoords.y = m_StandingStillSectionToDraw->position.y;
			canvas[m_VertexStartIndex + 5].texCoords.x = m_StandingStillSectionToDraw->position.x + texWidth;
			canvas[m_VertexStartIndex + 5].texCoords.y = m_StandingStillSectionToDraw->position.y + texHeight;
		}
	}
}
