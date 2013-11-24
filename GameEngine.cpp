#include "GameEngine.h"
#include "Globals.h"
#include "World.h"
#include "ResourceHandler.h"

sf::View playerStatsView;

GameEngine::GameEngine():
	window(sf::VideoMode(800, 600), "X1337", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize)
{
	
	sf::View gameView(sf::FloatRect(0,0,500,500));
	Globals::getInstance().setGameView(gameView);
	Globals::getInstance().getGameView().setViewport(sf::FloatRect(0, 0, 0.75f, 1));

	// mini-map (upper-right corner)
	playerStatsView = sf::View(sf::FloatRect(0,0,300,600));
	playerStatsView.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 1.0f));


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

			window.setView(Globals::getInstance().getGameView());
			this->world->draw();

			window.setView(playerStatsView);
			this->world->drawStats();

		}

		if(Globals::getInstance().getState() == Globals::MENU)
		{

		}

		window.display();
	}
}