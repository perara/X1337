#include "GameEngine.h"
#include "ResourceHandler.h"
#include "Menu.h"
#include <iostream>

GameEngine::GameEngine() :
window(sf::VideoMode(800, 600), "X1337", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize),
fullScreen(sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y))),
mainView(sf::View(sf::FloatRect(0, 0, 800, 600))),
playerStatsView(sf::View(sf::FloatRect(0, 0, 300, 600))),
menuGameDemoView(sf::View(sf::FloatRect(0, 0, 800, 600))),
resourceHandler(new ResourceHandler(window)),
event(sf::Event()),
mute(false),
timeStep(sf::seconds(1.0f / 60.0f)) // Set timestep to 60 FPS

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
	sf::Mouse::setPosition(sf::Vector2i((window.getView().getSize().x / 2), (window.getView().getSize().y / 2)), window); // Default mouse location
	window.setMouseCursorVisible(false);

	// Init Menu;
	menu = (std::unique_ptr<Menu>(new Menu(window, state, resourceHandler)));

	// Start Gameloop
	this->runGame();
}

void GameEngine::runGame()
{
	while (window.isOpen())
	{
		this->input(); // pollEvent
		this->elapsedTime += this->gameClock.restart();

		while (this->elapsedTime >= timeStep)
		{
			this->process();
			this->elapsedTime -= timeStep;
		}
		// Draw the game
		this->draw();

	}
}

void GameEngine::draw()
{
	window.clear(sf::Color::Black);


	if (getState() == GameState::GAME)
	{
		if (world->isGameOver())
		{
			setState(GameState::GAMEOVER);
			menu->updateCurrentOption();
		}
		window.setView(playerStatsView);
		this->world->drawStats();
		this->drawMute();

		window.setView(mainView);
		this->world->draw();
	}
	else if (getState() == GameState::MAIN_MENU ||
		getState() == GameState::STAGE_SELECT ||
		getState() == GameState::DIFFICULTY_SELECT ||
		getState() == GameState::CREDITS ||
		getState() == GameState::HIGHSCORE)
	{
		window.setView(fullScreen);
		this->world->draw();
		this->menu->draw();
		this->drawMute();
	}
	else if (getState() == GameState::HIGHSCORE)
	{
		menu->draw();
	}
	else if (getState() == GameState::PAUSE)
	{
		window.setView(playerStatsView);
		this->world->drawStats();

		window.setView(mainView);
		this->world->draw();

		window.setView(fullScreen);
		sf::RectangleShape darkOverLay(sf::Vector2f(window.getView().getSize()));
		darkOverLay.setFillColor(sf::Color(0, 0, 0, 150));
		darkOverLay.setPosition(0, 0);
		window.draw(darkOverLay);

		this->menu->draw();
		this->menu->drawPause((window.getView().getSize().y / 2), (window.getView().getSize().y / 2) * -1); // Small workaround so we dont have to take in offset into ->draw();
	}
	else if (getState() == GameState::GAMEOVER)
	{
		window.setView(playerStatsView);
		this->world->drawStats();

		window.setView(mainView);
		this->world->draw();

		window.setView(fullScreen);
		sf::RectangleShape darkOverLay(sf::Vector2f(window.getView().getSize()));
		darkOverLay.setFillColor(sf::Color(0, 0, 0, 150));
		darkOverLay.setPosition(0, 0);
		window.draw(darkOverLay);

		this->menu->draw();
		this->menu->drawPause((window.getView().getSize().y / 2), (window.getView().getSize().y / 2) * -1); // Small workaround so we dont have to take in offset into ->draw();
	}

	window.display();

}

void GameEngine::process()
{
	if (getState() == GameState::GAME)
	{
		// Process Scene
		this->world->input(this->event);
		this->world->process();
	}
	else if (getState() == GameState::INIT_GAME)
	{
		setState(GameState::GAME);
		world->stopSound();
		world.reset(new World(
			window,
			resourceHandler,
			timeStep,
			false,
			menu->getStageSelectOption(),
			menu->getHardmodeSelected(),
			(menu->getStageSelectOption()-1 == ResourceHandler::Scripts::DEATH_STAR) ?
			resourceHandler->getSound(ResourceHandler::Sound::DEATH_STAR_THEME) :
			resourceHandler->getSound(ResourceHandler::Sound::INGAME)));
	}
	else if (getState() == GameState::INIT_MAIN_MENU)
	{
		setState(GameState::MAIN_MENU);
		if (world != nullptr) world->stopSound();
		world.reset(new World(window,
			resourceHandler,
			timeStep,
			true,
			-1,
			false,
			resourceHandler->getSound(ResourceHandler::Sound::MENU_SONG)));
	}
	else if (getState() == GameState::GAMEOVER)
	{
		world->stopSound();
	}
	else if (
		getState() == GameState::MAIN_MENU ||
		getState() == GameState::STAGE_SELECT ||
		getState() == GameState::DIFFICULTY_SELECT ||
		getState() == GameState::CREDITS ||
		getState() == GameState::HIGHSCORE)
	{
		this->world->process();
		this->menu->process();
	}
}

void GameEngine::input()
{
	while (window.pollEvent(this->event))
	{

		/* Input event for each of the STATES */
		if (getState() == GameState::MAIN_MENU
			|| getState() == GameState::STAGE_SELECT
			|| getState() == GameState::DIFFICULTY_SELECT
			|| getState() == GameState::PAUSE
			|| getState() == GameState::HIGHSCORE
			|| getState() == GameState::GAMEOVER
			|| getState() == GameState::CREDITS)
		{
			this->menu->input(this->event);
		}


		if (this->event.type == sf::Event::Closed)
		{
			this->window.close();
			exit(EXIT_SUCCESS);
		}



		/* GENERAL Key Events*/
		if (this->event.type == sf::Event::KeyReleased)
		{
			if (this->event.key.code == sf::Keyboard::Escape){
				if (getState() == GameState::GAME)
				{
					setState(GameState::PAUSE);
				}
				else if (getState() == GameState::PAUSE)
				{
					setState(GameState::GAME);

				}
				menu->updateCurrentOption();
			}

			else if (this->event.key.code == sf::Keyboard::M) // Sound mute button
			{
				mute = !mute;
				resourceHandler->muteSound(mute);
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


std::shared_ptr<ResourceHandler>& GameEngine::getResourceHandler()
{
	return this->resourceHandler;
}

void GameEngine::drawMute()
{
	// Kinda bad pracice to reinitialize these on each frame. But for readability ill have it since 
	// since i basicly have "unlimited" power
	sf::Text txtMute;
	txtMute.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	txtMute.setString("m");
	txtMute.setColor(sf::Color(139, 137, 137));
	txtMute.setCharacterSize(15);
	txtMute.setPosition(window.getView().getSize().x - 100, window.getView().getSize().y - 24);
	window.draw(txtMute);

	sf::Sprite sprMute;
	if (mute)
	{
		sprMute.setTexture(resourceHandler->getTexture(ResourceHandler::Texture::AUDIO_OFF));
	}
	else
	{
		sprMute.setTexture(resourceHandler->getTexture(ResourceHandler::Texture::AUDIO_ON));
	}
	sprMute.setScale(0.20f, 0.20f);
	sprMute.setPosition(txtMute.getPosition().x + 25, txtMute.getPosition().y);
	window.draw(sprMute);
}