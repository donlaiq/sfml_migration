#include "FireballGraphics.h"
#include "Animator.h"
#include "FireballUpdate.h"

void FireballGraphics::assemble(VertexArray& canvas, shared_ptr<Update> genericUpdate, IntRect texCoords)
{
	shared_ptr<FireballUpdate> fu = static_pointer_cast<FireballUpdate>(genericUpdate);
	m_Position = fu->getPositionPointer();
	m_FacingRight = fu->getFacingRightPointer();

	m_Animator = new Animator(texCoords.position.x, texCoords.position.y, 3, texCoords.size.x * 3, texCoords.size.y, 6);

	// Get the first frame of animation
	m_SectionToDraw = m_Animator->getCurrentFrame(false);

	m_VertexStartIndex = canvas.getVertexCount();
	canvas.resize(canvas.getVertexCount() + 6);

	const int uPos = texCoords.position.x;
	const int vPos = texCoords.position.y;
	const int texWidth = texCoords.size.x;
	const int texHeight = texCoords.size.y;

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

void FireballGraphics::draw(VertexArray& canvas)
{
	const Vector2f position = m_Position->position;
	const Vector2f scale = m_Position->size;

	canvas[m_VertexStartIndex].position = position;
	canvas[m_VertexStartIndex + 1].position = position + Vector2f(scale.x, 0);
	canvas[m_VertexStartIndex + 2].position = position + scale;
	canvas[m_VertexStartIndex + 3].position = position + scale;
	canvas[m_VertexStartIndex + 4].position = position;
	canvas[m_VertexStartIndex + 5].position = position + Vector2f(0, scale.y);

	if(*m_FacingRight)
	{
		m_SectionToDraw = m_Animator->getCurrentFrame(false);
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
	else
	{
		// Doesn't make much differnce to fire which order the frame are drawn, but front must be ar front duh!!!!	
		m_SectionToDraw = m_Animator->getCurrentFrame(true);
		//reversed
		const int uPos = m_SectionToDraw->position.x;
		const int vPos = m_SectionToDraw->position.y;
		const int texWidth = m_SectionToDraw->size.x;
		const int texHeight = m_SectionToDraw->size.y;

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
}
