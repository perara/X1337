#include "GameEngine.h"
#include "Globals.h"
#include "World.h"
#include "ResourceHandler.h"

GameEngine::GameEngine():
	window(sf::VideoMode(800, 600), "X1337", sf::Style::Titlebar | sf::Style::Close)
{
	sf::View gameView(sf::FloatRect(0,0,500,500));
	Globals::getInstance().setGameView(gameView);
	window.setView(Globals::getInstance().getGameView());

	this->window.setFramerateLimit(120);

	// Init and set resourceHandler
	Globals::getInstance().setResourceHandler(new ResourceHandler(window));
	Globals::getInstance().getResourceHandler()->init();

	// Init World
	world = new World(window);

	// Set timeStep to 60 fps
	Globals::getInstance().setTimeStep(sf::seconds(1.0f/60.0f));

	// Initial Game State
	Globals::getInstance().setState(Globals::getInstance().GAME);		// Set gamestate to Game


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

		while(this->elapsedTime >=  Globals::getInstance().getTimeStep())
		{

			if(Globals::getInstance().getState() == Globals::GAME)
			{
				// Process Scene
				this->world->process();

			}

			if(Globals::getInstance().getState() == Globals::MENU)
			{

			}

			this->elapsedTime -= Globals::getInstance().getTimeStep();

		}

		window.clear(sf::Color::Black);

		if(Globals::getInstance().getState() == Globals::GAME)
		{
			// Draw Game
			this->world->draw();

		}

		if(Globals::getInstance().getState() == Globals::MENU)
		{

		}

		window.display();
	}
}