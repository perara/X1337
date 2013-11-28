#include "GameEngine.h"
#include "Globals.h"
#include "World.h"
#include "ResourceHandler.h"
#include "Menu.h"

#include <iostream>

GameEngine::GameEngine():
	window(sf::VideoMode(800, 600), "X1337", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize),
	fullScreen(sf::View(sf::FloatRect(0,0,window.getSize().x,window.getSize().y))),
	mainView (sf::View(sf::FloatRect(0,0,800,600))),
	playerStatsView(sf::View(sf::FloatRect(0,0,300,600))),
	menuGameDemoView(sf::View (sf::FloatRect(0,0,800,600)))


{
	// Window configuration
	this->window.setFramerateLimit(120);

	//************************************//
	//*********View Declaration***********//
	//************************************//
	fullScreen.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	window.setView(fullScreen);

	mainView.setViewport(sf::FloatRect(0, 0, 0.75f, 1));
	playerStatsView.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 1.0f));
	menuGameDemoView.setViewport(sf::FloatRect(0.40f, 0.40f, 0.25f, 0.25f));



	// Init and set resourceHandler
	Globals::getInstance().setResourceHandler(std::shared_ptr<ResourceHandler>(new ResourceHandler(window)));
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
	world = std::unique_ptr<World>(new World(window));
	world->setDemo(true);

	// Init Menu
	menu = std::unique_ptr<Menu>(new Menu(window));


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
			if(GlobalState == Globals::State::GAME)
			{
				// Process Scene
				this->world->process();
				this->world->input(this->event); // todo should be in polLEvent
			}
			else if(GlobalState == Globals::INIT_GAME)
			{
				Globals::getInstance().getResourceHandler()->getSound(ResourceHandler::Sound::MENU_SONG).stop();
				this->world->setDemo(false);
				this->world->reset();
				this->world->init(menu->getStageSelectOption());
				Globals::getInstance().setState(Globals::State::GAME);
			}
			else if(GlobalState == Globals::State::INIT_MAIN_MENU)
			{
				Globals::getInstance().setState(Globals::State::MAIN_MENU);
				std::unique_ptr<World> world = std::unique_ptr<World>(new World(window));
				this->world->setDemo(true);
			}
			else if(GlobalState == Globals::State::MAIN_MENU  || GlobalState == Globals::State::STAGE_SELECT)
			{
				this->world->process();
				this->menu->process();
			}
			this->elapsedTime -= Globals::getInstance().getTimeStep();

		}

		window.clear(sf::Color::Black);


		if(GlobalState == Globals::State::GAME)
		{
			window.setView(playerStatsView);
			this->world->drawStats();

			window.setView(mainView);
			this->world->draw();
		}
		else if(GlobalState == Globals::State::INIT_GAME)
		{

		}
		else if(GlobalState == Globals::State::MAIN_MENU || GlobalState == Globals::State::STAGE_SELECT)
		{
			window.setView(fullScreen);
			this->world->draw();
			this->menu->draw();
		}
		else if(GlobalState == Globals::State::PAUSE)
		{
			window.setView(playerStatsView);
			this->world->drawStats();

			window.setView(mainView);
			this->world->draw();

			window.setView(fullScreen);
			sf::RectangleShape darkOverLay(sf::Vector2f(window.getSize()));
			darkOverLay.setFillColor(sf::Color(0,0,0,150));
			darkOverLay.setPosition(0,0);
			window.draw(darkOverLay);

			this->menu->draw();
			this->menu->drawPause(( window.getSize().y / 2),( window.getSize().y / 2) * -1); // Small workaround so we dont have to take in offset into ->draw();

		}

		window.display();
	}
}

void GameEngine::pollInput()
{
	while (window.pollEvent(this->event))
	{

		/* Input event for each of the STATES */
		if(GlobalState == Globals::State::GAME)
		{
			this->menu->input(this->event);
		}
		else if(GlobalState == Globals::State::INIT_GAME)
		{
		}
		else if(GlobalState == Globals::State::MAIN_MENU || GlobalState == Globals::State::STAGE_SELECT || GlobalState == Globals::State::PAUSE)
		{
			this->menu->input(this->event);
		}


		if(this->event.type == sf::Event::Closed)
		{
			this->window.close();
		}



		/* GENERAL Key Events*/
		if(this->event.type == sf::Event::KeyReleased)
		{
			if(this->event.key.code == sf::Keyboard::Escape){
				if(GlobalState == Globals::State::GAME)
				{
					Globals::getInstance().setState(Globals::State::PAUSE);
				}
				else if(GlobalState == Globals::State::PAUSE)
				{
					Globals::getInstance().setState(Globals::State::GAME);
				}
			}
		}

	}
}