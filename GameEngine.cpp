#include "GameEngine.h"
#include "Globals.h"
#include "World.h"
#include "ResourceHandler.h"
#include "Menu.h"

GameEngine::GameEngine():
	window(sf::VideoMode(800, 600), "X1337", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize)
{
	// Window configuration
	this->window.setFramerateLimit(120);

	//************************************//
	//*********View Declaration***********//
	//************************************//
	fullScreen = sf::View(sf::FloatRect(0,0,window.getSize().x,window.getSize().y));
	fullScreen.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));


	// Game Views
	{
		// Main View
		sf::View gameView(sf::FloatRect(0,0,800,600));
		Globals::getInstance().setGameView(gameView); // TODO not global
		Globals::getInstance().getGameView().setViewport(sf::FloatRect(0, 0, 0.75f, 1));

		// Sidepanel view
		playerStatsView = sf::View(sf::FloatRect(0,0,300,600));
		playerStatsView.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 1.0f));
	}

	// Menu Views
	{
		menuGameDemoView = sf::View (sf::FloatRect(0,0,800,600));
		menuGameDemoView.setViewport(sf::FloatRect(0.40f, 0.40f, 0.25f, 0.25f));
	}


	// Init and set resourceHandler
	Globals::getInstance().setResourceHandler(new ResourceHandler(window));
	Globals::getInstance().getResourceHandler()->init();

	// Set timeStep to 60 fps
	Globals::getInstance().setTimeStep(sf::seconds(1.0f/60.0f));

	// Initial Game State
	Globals::getInstance().setState(Globals::getInstance().MAIN_MENU);		// Set gamestate to Game

	// Initialize Event
	this->event = sf::Event();

	// Set mouse properties
	sf::Mouse::setPosition(sf::Vector2i((window.getSize().x / 2), (window.getSize().y / 2)), window); // Default mouse location
	window.setMouseCursorVisible(false);

	// Init World
	this->world = new World(window);
	this->world->setDemo(true);

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
			switch(Globals::getInstance().getState())
			{
			case Globals::INIT_GAME: // Init game is initiated whenever new game is pressed
				this->world->setDemo(false);
				this->world->reset();
				Globals::getInstance().setState(Globals::GAME);

				break;
			case Globals::GAME:
				// Process Scene
				this->world->process();
				this->world->input(this->event); // todo should be in polLEvent

				break;
			case Globals::MAIN_MENU:
				this->world->process();
				this->menu->process();
				break;
			}

			this->elapsedTime -= Globals::getInstance().getTimeStep();

		}

		window.clear(sf::Color::Black);


		switch(Globals::getInstance().getState())
		{
		case Globals::GAME:
			window.setView(Globals::getInstance().getGameView());
			this->world->draw();

			window.setView(playerStatsView);
			this->world->drawStats();
			break;
		case Globals::MAIN_MENU:
			window.setView(fullScreen);
			this->world->draw();
			this->menu->draw();

			break;

		}




		window.display();
	}
}

void GameEngine::pollInput()
{
	while (window.pollEvent(this->event))
	{

		/* Input event for each of the STATES */
		switch(Globals::getInstance().getState())
		{
		case Globals::GAME:
			this->menu->input(this->event);
			break;
		case Globals::MAIN_MENU:
			this->menu->input(this->event);
			break;

		}

		if(this->event.type == sf::Event::Closed)
		{
			this->window.close();
		}



		/* GENERAL Key Events*/
		if(this->event.type == sf::Event::KeyReleased)
		{
			if(this->event.key.code == sf::Keyboard::Escape){
				this->window.close();
			}
		}

	}
}