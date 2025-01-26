#include "CameraGraphics.h"
#include "CameraUpdate.h"

CameraGraphics::CameraGraphics(RenderWindow* window, Texture* texture, Vector2f viewSize, FloatRect viewport) : m_Text{m_Font}, 
																												m_BackgroundTexture("graphics/backgroundTexture.png"),
																												m_BackgroundSprite{m_BackgroundTexture}, 
																												m_BackgroundSprite2{m_BackgroundTexture}
{
	m_Window = window;
	m_Texture = texture;

	m_View.setSize(viewSize);
	m_View.setViewport(viewport);

	// The mini map viewport is less than 1
	if(viewport.size.x < 1)
		m_IsMiniMap = true;
	else
	{
		// Only the full screen camera has the time text
		m_Font.openFromFile("fonts/KOMIKAP_.ttf");
		m_Text.setFont(m_Font);
		m_Text.setFillColor(Color(255, 0, 0, 255));
		m_Text.setScale({0.2f, 0.2f});
	}

	m_BackgroundSprite.setPosition({0, -200});

	// Initialize the shader
	m_Shader.loadFromFile("shaders/glslsandbox109644", sf::Shader::Type::Fragment);
	if(!m_Shader.isAvailable())
		std::cout << "The shader is not available\n";
	m_Shader.setUniform("resolution", sf::Vector2f(2500, 2500));
	m_ShaderClock.restart();
}

void CameraGraphics::assemble(VertexArray& canvas, shared_ptr<Update> genericUpdate, IntRect texCoords)
{
	shared_ptr<CameraUpdate> cameraUpdate = static_pointer_cast<CameraUpdate>(genericUpdate);
	m_Position = cameraUpdate->getPositionPointer();
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

float* CameraGraphics::getTimeConnection()
{
	return &m_Time;
}

void CameraGraphics::draw(VertexArray& canvas)
{
	m_View.setCenter(m_Position->position);

	Vector2f startPosition;
	startPosition.x = m_View.getCenter().x - m_View.getSize().x / 2;
	startPosition.y = m_View.getCenter().y - m_View.getSize().y / 2;

	Vector2f scale;
	scale.x = m_View.getSize().x;
	scale.y = m_View.getSize().y;

	canvas[m_VertexStartIndex].position = startPosition;
	canvas[m_VertexStartIndex + 1].position = startPosition + Vector2f(scale.x, 0);
	canvas[m_VertexStartIndex + 2].position = startPosition + scale;
	canvas[m_VertexStartIndex + 3].position = startPosition + scale;
	canvas[m_VertexStartIndex + 4].position = startPosition;
	canvas[m_VertexStartIndex + 5].position = startPosition + Vector2f(0, scale.y);

	if(m_IsMiniMap)
	{
		if(m_View.getSize().x < MAX_WIDTH && m_Position->size.x > 1)
			m_View.zoom(m_Position->size.x);
		else if(m_View.getSize().x > MIN_WIDTH && m_Position->size.x < 1)
			m_View.zoom(m_Position->size.x);
	}

	m_Window->setView(m_View);

	// Background stuff
	Vector2f movement;
	movement.x = m_Position->position.x - m_PlayersPreviousPosition.x;
	movement.y = m_Position->position.y - m_PlayersPreviousPosition.y;

	if(m_BackgroundsAreFlipped)
	{
		m_BackgroundSprite2.setPosition({m_BackgroundSprite2.getPosition().x + movement.x / 6, m_BackgroundSprite2.getPosition().y + movement.y / 6});
		m_BackgroundSprite.setPosition({m_BackgroundSprite2.getPosition().x + m_BackgroundSprite2.getTextureRect().size.x, m_BackgroundSprite2.getPosition().y});

		if(m_Position->position.x > m_BackgroundSprite.getPosition().x + m_BackgroundSprite.getTextureRect().size.x / 2)
		{
			m_BackgroundsAreFlipped = !m_BackgroundsAreFlipped;
			m_BackgroundSprite2.setPosition(m_BackgroundSprite.getPosition());
		}
	}
	else
	{
		m_BackgroundSprite.setPosition({m_BackgroundSprite.getPosition().x - movement.x / 6, m_BackgroundSprite.getPosition().y + movement.y / 6});
		m_BackgroundSprite2.setPosition({m_BackgroundSprite.getPosition().x + m_BackgroundSprite.getTextureRect().size.x, m_BackgroundSprite.getPosition().y});

		if(m_Position->position.x > m_BackgroundSprite2.getPosition().x + m_BackgroundSprite2.getTextureRect().size.x / 2)
		{
			m_BackgroundsAreFlipped = !m_BackgroundsAreFlipped;
			m_BackgroundSprite.setPosition(m_BackgroundSprite2.getPosition());
		}
	}

	m_PlayersPreviousPosition.x = m_Position->position.x;
	m_PlayersPreviousPosition.y = m_Position->position.y;

	// Set the other parameters who need to be updated every frame
	m_Shader.setUniform("time", m_ShaderClock.getElapsedTime().asSeconds());

	sf::Vector2i mousePos = m_Window->mapCoordsToPixel(m_Position->position);
	m_Shader.setUniform("mouse", sf::Vector2f(mousePos.x, mousePos.y + 1000));

	if(m_ShaderClock.getElapsedTime().asSeconds() > 10)
	{
		m_ShaderClock.restart();
		m_ShowShader = !m_ShowShader;
	}

	if(!m_ShowShader)
	{
		m_Window->draw(m_BackgroundSprite, &m_Shader);
		m_Window->draw(m_BackgroundSprite2, &m_Shader);
	}
	else // Show the parallax background
	{
		m_Window->draw(m_BackgroundSprite);
		m_Window->draw(m_BackgroundSprite2);
	}

	// Draw the time UI only in thge main camera
	if(!m_IsMiniMap)
	{
		m_Text.setString(std::to_string(m_Time));
		m_Text.setPosition(m_Window->mapPixelToCoords(Vector2i(5, 5)));
		m_Window->draw(m_Text);
	}

	// Draw the main canvas
	m_Window->draw(canvas, m_Texture);
}
