#include "RainGraphics.h"
#include "Animator.h"

RainGraphics::RainGraphics(FloatRect* playerPosition, float horizontalOffset, float verticalOffset, int rainCoveragePerObject)
{
	m_PlayerPosition = playerPosition;
	m_HorizontalOffset = horizontalOffset;
	m_VerticalOffset = verticalOffset;

	m_Scale.x = rainCoveragePerObject;
	m_Scale.y = rainCoveragePerObject;
}

void RainGraphics::assemble(VertexArray& canvas, shared_ptr<Update> genericUpdate, IntRect texCoords)
{
	m_Animator = new Animator(texCoords.position.x, texCoords.position.y, 4, texCoords.size.x * 4, texCoords.size.y, 8);

	m_VertexStartIndex = canvas.getVertexCount();
	canvas.resize(canvas.getVertexCount() + 6);
}

void RainGraphics::draw(VertexArray& canvas)
{
	const Vector2f position = m_PlayerPosition->position - Vector2f(m_Scale.x / 2 + m_HorizontalOffset, m_Scale.y / 2 + m_VerticalOffset);

	// Move the rain to keep up with the player
	canvas[m_VertexStartIndex].position = position;
	canvas[m_VertexStartIndex + 1].position = position + Vector2f(m_Scale.x, 0);
	canvas[m_VertexStartIndex + 2].position = position + m_Scale;
	canvas[m_VertexStartIndex + 3].position = position + m_Scale;
	canvas[m_VertexStartIndex + 4].position = position;
	canvas[m_VertexStartIndex + 5].position = position + Vector2f(0, m_Scale.y);

	// Cycle through the frames
	m_SectionToDraw = m_Animator->getCurrentFrame(false);

	// Remember the section of the texture to draw
	const int uPos = m_SectionToDraw->position.x;
	const int vPos = m_SectionToDraw->position.y;
	const int texWidth = m_SectionToDraw->size.x;
	const int texHeight = m_SectionToDraw->size.y;

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
