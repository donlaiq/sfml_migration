#include "Bat.h"
#include "Ball.h"
#include <sstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>

int main()
{
	// Create a video mode object
	VideoMode vm({1920, 1080});

	// Create and open a window for the game
	RenderWindow window(vm, "Pong", State::Fullscreen);
	int score = 0;
	int lives = 3;

	// Create a bat at the bottom center of the screen
	Bat bat{1920 / 2, 1080 - 20};

	// Create a ball
	Ball ball{1920 / 2, 0};
	
	// A cool retro-style font
	Font font;
	if(!font.openFromFile("fonts/DS-DIGIT.TTF"))
		return 1;
	
	// Create a Text object called hud
	Text hud{font};
	
	// Set the font to our retro-style
	hud.setFont(font);
	// Make it nice and big
	hud.setCharacterSize(75);
	// Choose a color
	hud.setFillColor(Color::White);
	hud.setPosition({20, 20});
	// Here is our clock for timing everything
	Clock clock;

	while(window.isOpen())
	{
		/*
		Handle the player input
		******************************
		******************************	
		******************************
		*/

		while(const std::optional event = window.pollEvent())
		{
			if(event->is<Event::Closed>())
				// Quit the game when the window is closed
				window.close();
		}

		// Handle the player quitting
		if(Keyboard::isKeyPressed(Keyboard::Scancode::Escape))
			window.close();

		// Handle the pressing and releasing of the arrow keys
		if(Keyboard::isKeyPressed(Keyboard::Scancode::Left))
			bat.moveLeft();
		else
			bat.stopLeft();
		
		if(Keyboard::isKeyPressed(Keyboard::Scancode::Right))
			bat.moveRight();
		else
			bat.stopRight();

		/*
		Update the bat, the ball and the HUD
		******************************
		******************************
		******************************
		*/

		// Update the delta time
		Time dt = clock.restart();
		bat.update(dt);
		ball.update(dt);

		// Update the HUD text
		std::stringstream ss;
		ss << "Score:" << score << "  Lives:" << lives;
		hud.setString(ss.str());

		// Handle ball hitting the bottom
		if(ball.getPosition().position.y > window.getSize().y)
		{
			// Reverse the ball direction
			ball.reboundBottom();
			// Remove a life
			lives--;
			// Check for zero lives
			if(lives < 1)
			{
				// Reset the score
				score = 0;
				// Reset the lives
				lives = 3;
			}
		}

		// Handle ball hitting top
		if(ball.getPosition().position.y  < 0)
		{
			ball.reboundBatOrTop();
			// Add a point to the player score
			score++;
		}

		// Handle ball hitting sides
		if(ball.getPosition().position.x < 0 || ball.getPosition().position.x + ball.getPosition().size.x > window.getSize().x)
			ball.reboundSides();

		// Has the ball hit the bat?
		if(ball.getPosition().findIntersection(bat.getPosition()))
			ball.reboundBatOrTop();


		/*
		Draw the bat, the ball and the HUD
		******************************
		******************************
		******************************
		*/

		window.clear();
		window.draw(hud);
		window.draw(bat.getShape());
		window.draw(ball.getShape());
		window.display();
	}
}

