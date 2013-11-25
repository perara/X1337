#include "GameEngine.h"
#include "Globals.h"
#include "World.h"
#include "ResourceHandler.h"
#include "Menu.h"

sf::View playerStatsView;

GameEngine::GameEngine():
	window(sf::VideoMode(800, 600), "X1337", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize)
{
	// Window configuration
	this->window.setFramerateLimit(120);

	// Configure game view
	sf::View gameView(sf::FloatRect(0,0,500,500));
	Globals::getInstance().setGameView(gameView);
	Globals::getInstance().getGameView().setViewport(sf::FloatRect(0, 0, 0.75f, 1));

	// Sidepanel view
	playerStatsView = sf::View(sf::FloatRect(0,0,300,600));
	playerStatsView.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 1.0f));

	// Init and set resourceHandler
	Globals::getInstance().setResourceHandler(new ResourceHandler(window));
	Globals::getInstance().getResourceHandler()->init();

	// Set timeStep to 60 fps
	Globals::getInstance().setTimeStep(sf::seconds(1.0f/60.0f));

	// Initial Game State
	Globals::getInstance().setState(Globals::getInstance().MENU);		// Set gamestate to Game

	// Initialize Event
	this->event = sf::Event();

	// Set mouse properties
	sf::Mouse::setPosition(sf::Vector2i((window.getSize().x / 2), (window.getSize().y / 2)), window); // Default mouse location
	window.setMouseCursorVisible(false);

	// Init World
	this->world = new World(window);

	// Init Menu
	this->menu = new Menu(window);

	// Start Gameloop
	this->runGame();


}

void GameEngine::runGame()
{
	while(window.isOpen())
	{
		this->pollInput(); // pollEvent
		this->elapsedTime += this->gameClock.restart();

		while(this->elapsedTime >=  Globals::getInstance().getTimeStep())
		{
			if(Globals::getInstance().getState() == Globals::GAME)
			{
				// Process Scene
				this->world->process();
				this->world->input(this->event); // todo should be in polLEvent
			}

			if(Globals::getInstance().getState() == Globals::MENU)
			{
				this->menu->process();
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
			this->menu->draw();
		}

		window.display();
	}
}

void GameEngine::pollInput()
{
	while (window.pollEvent(this->event))
	{
		this->menu->input(this->event);

		if(this->event.type == sf::Event::Closed)
		{
			this->window.close();
		}

		/* Key Events*/
		if(this->event.type == sf::Event::KeyReleased)
		{
			if(this->event.key.code == sf::Keyboard::Escape){
				this->window.close();
			}
		}

	}
}