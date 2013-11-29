#include "GameEngine.h"
#include "ResourceHandler.h"
#include "Menu.h"
#include <iostream>

GameEngine::GameEngine():
	window(sf::VideoMode(800, 600), "X1337", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize),
	fullScreen(sf::View(sf::FloatRect(0,0,window.getSize().x,window.getSize().y))),
	mainView (sf::View(sf::FloatRect(0,0,800,600))),
	playerStatsView(sf::View(sf::FloatRect(0,0,300,600))),
	menuGameDemoView(sf::View (sf::FloatRect(0,0,800,600))),
	resourceHandler(new ResourceHandler(window)),
	event(sf::Event()),
	timeStep(sf::seconds(1.0f/60.0f)) // Set timestep to 60 FPS

{
	// Window configuration
	window.setFramerateLimit(120);

	//************************************//
	//*********View Declaration***********//
	//************************************//
	fullScreen.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	window.setView(fullScreen);

	mainView.setViewport(sf::FloatRect(0, 0, 0.75f, 1));
	playerStatsView.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 1.0f));
	menuGameDemoView.setViewport(sf::FloatRect(0.40f, 0.40f, 0.25f, 0.25f));

	// Init resourceHandler
	resourceHandler->init();

	// Initial Game State
	setState(GameEngine::State::INIT_MAIN_MENU);  // Set gamestate to init_main_menu

	// Set mouse properties
	sf::Mouse::setPosition(sf::Vector2i((window.getSize().x / 2), (window.getSize().y / 2)), window); // Default mouse location
	window.setMouseCursorVisible(false);

	// Init Menu;
	menu = (std::unique_ptr<Menu>(new Menu(window, state, resourceHandler)));

	// Start Gameloop
	this->runGame();
}

void GameEngine::runGame()
{
	while(window.isOpen())
	{
		this->pollInput(); // pollEvent
		this->elapsedTime += this->gameClock.restart();

		while(this->elapsedTime >= timeStep)
		{
			if(getState() == GameState::GAME)
			{
				// Process Scene
				this->world->process();
				this->world->input(this->event); // todo should be in polLEvent
			}

			else if(getState() == GameState::INIT_GAME)
			{
				resourceHandler->getSound(ResourceHandler::Sound::MENU_SONG).stop();
				setState(GameState::GAME);
				world.reset(new World(window, resourceHandler, timeStep));
				world->init(false, menu->getStageSelectOption());
			}

			else if(getState() == GameState::INIT_MAIN_MENU)
			{
				resourceHandler->getSound(ResourceHandler::Sound::MENU_SONG).play();
				resourceHandler->getSound(ResourceHandler::Sound::MENU_SONG).setLoop(true);
				setState(GameState::MAIN_MENU);
				world.reset(new World(window, resourceHandler, timeStep));
				world->init(true);
			}

			else if(getState() == GameState::MAIN_MENU  || getState() == GameState::STAGE_SELECT)
			{
				this->world->process();
				this->menu->process();
			}
			this->elapsedTime -= timeStep;
		}

		window.clear(sf::Color::Black);


		if(getState() == GameState::GAME)
		{
			if(world->isGameOver())
			{
				setState(GameState::GAMEOVER);
				menu->updateCurrentOption();
			}
			window.setView(playerStatsView);
			this->world->drawStats();

			window.setView(mainView);
			this->world->draw();
		}
		else if(getState() == GameState::INIT_GAME)
		{

		}
		else if(getState() == GameState::MAIN_MENU || getState() == GameState::STAGE_SELECT)
		{
			window.setView(fullScreen);
			this->world->draw();
			this->menu->draw();
		}
		else if(getState() == GameState::PAUSE)
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
		else if(getState() == GameState::GAMEOVER)
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
		if(getState() == GameState::GAME)
		{

		}
		else if(getState() == GameState::INIT_GAME)
		{
		}
		else if(getState() ==GameState::MAIN_MENU || getState() == GameState::STAGE_SELECT || getState() == GameState::PAUSE || getState() == GameState::GAMEOVER)
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
				if(getState() == GameState::GAME)
				{
					setState(GameState::PAUSE);
				}
				else if(getState() == GameState::PAUSE)
				{
					setState(GameState::GAME);

				}
				menu->updateCurrentOption();
			}
		}

	}
}

// State getter/setter
void GameEngine::setState(GameState state)
{
	this->state = state;
}

GameState& GameEngine::getState()
{
	return this->state;
}


std::unique_ptr<ResourceHandler>& GameEngine::getResourceHandler()
{
	return this->resourceHandler;
}
