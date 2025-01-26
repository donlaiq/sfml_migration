#include "PlatformGraphics.h"
#include "PlatformUpdate.h"

void PlatformGraphics::draw(VertexArray& canvas)
{
	const Vector2f& position = m_Position->position;
	const Vector2f& scale = m_Position->size;

	canvas[m_VertexStartIndex].position = position;
	canvas[m_VertexStartIndex + 1].position = position + Vector2f(scale.x, 0);
	canvas[m_VertexStartIndex + 2].position = position + scale;
	canvas[m_VertexStartIndex + 3].position = position + scale;
	canvas[m_VertexStartIndex + 4].position = position;
	canvas[m_VertexStartIndex + 5].position = position + Vector2f(0, scale.y);
}

void PlatformGraphics::assemble(VertexArray& canvas, shared_ptr<Update> genericUpdate, IntRect texCoords)
{
	shared_ptr<PlatformUpdate> platformUpdate = static_pointer_cast<PlatformUpdate>(genericUpdate);
	m_Position = platformUpdate->getPositionPointer();
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
