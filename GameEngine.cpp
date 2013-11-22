#include "GameEngine.h"

GameEngine::GameEngine():
	window(sf::VideoMode(500, 500), "X1337", sf::Style::Titlebar | sf::Style::Close),
	world(window)
{
	Config::getInstance().timeStep =  sf::seconds(1.0f/60.0f);

	// Set mouse properties
	sf::Mouse::setPosition(sf::Vector2i((window.getSize().x / 2), (window.getSize().y / 2)), window); // Default mouse location
	window.setMouseCursorVisible(false);

	// Initial Configuration
	Config::getInstance().state = Config::getInstance().GAME;		// Set gamestate to Game
	if (!Config::getInstance().font.loadFromFile("COMICATE.TTF"))
		LOGE("Error, could not load font");

	// Start Gameloop
	this->runGame();
}

void GameEngine::runGame()
{
	while(window.isOpen())
	{
		Config::getInstance().elapsedTime += Config::getInstance().gameClock.restart();

		while(Config::getInstance().elapsedTime >=  Config::getInstance().timeStep)
		{

			if(Config::getInstance().state == Config::GAME)
			{
				// Process Scene
				this->world.process();

			}

			if(Config::getInstance().state == Config::MENU){

			}

			Config::getInstance().elapsedTime -= Config::getInstance().timeStep;

		}
		// Draw Scene'
		window.clear(sf::Color::Black);
		this->world.draw();
		window.display();
	}
}