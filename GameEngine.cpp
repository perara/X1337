#include "GameEngine.h"

GameEngine::GameEngine():
	window(sf::VideoMode(500, 500), "X1337", sf::Style::Titlebar | sf::Style::Close),
	world(window)
{

	// Set timeStep to 60 fps
	Config::getInstance().timeStep =  sf::seconds(1.0f/60.0f);

	// Initial Game State
	Config::getInstance().state = Config::getInstance().GAME;		// Set gamestate to Game

	// Set mouse properties
	sf::Mouse::setPosition(sf::Vector2i((window.getSize().x / 2), (window.getSize().y / 2)), window); // Default mouse location
	window.setMouseCursorVisible(false);

	// Start Gameloop
	this->runGame();
}

void GameEngine::runGame()
{
	while(window.isOpen())
	{
		this->elapsedTime += this->gameClock.restart();

		while(this->elapsedTime >=  Config::getInstance().timeStep)
		{

			if(Config::getInstance().state == Config::GAME)
			{
				// Process Scene
				this->world.process();

			}

			if(Config::getInstance().state == Config::MENU)
			{

			}

			this->elapsedTime -= Config::getInstance().timeStep;

		}


		if(Config::getInstance().state == Config::GAME)
		{
			// Draw Game
			window.clear(sf::Color::Black);
			this->world.draw();

		}

		if(Config::getInstance().state == Config::MENU)
		{

		}

		window.display();
	}
}