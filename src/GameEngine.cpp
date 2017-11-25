#include "../include/GameEngine.h"
#include "../include/ResourceHandler.h"
#include "../include/Menu.h"
#include "../include/log.h"

/// <summary>
/// Initializes a new instance of the <see cref="GameEngine"/> class.
/// </summary>
GameEngine::GameEngine() :
// Declare the Window
window(sf::VideoMode(800, 600), "X1337", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize),
timeStep(sf::seconds(1.0f / 240.f)), // Set timestep to 60 FPS



// Declare all of the views
fullView(sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y))),
mainView(sf::View(sf::FloatRect(0, 0, 800, 600))),
playerBar(sf::View(sf::FloatRect(0, 0, 800, 30))),
menuGameDemoView(sf::View(sf::FloatRect(0, 0, 800, 600))),

// Create a new Resource Handler (smart_ptr)
resourceHandler(new ResourceHandler(window)),

event(sf::Event()),
mute(false),
fullscreen(false),

state(GameEngine::State::INIT_MAIN_MENU)
{
	// Window configuration
	//window.setFramerateLimit(120);

	//####################################//
	//#########View Declaration###########//
	//####################################//
	fullView.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	window.setView(fullView);

	mainView.setViewport(sf::FloatRect(0, 0, 1, 0.95f));
	playerBar.setViewport(sf::FloatRect(0, 0.95f, 1.0f, 0.05f));
	menuGameDemoView.setViewport(sf::FloatRect(0.40f, 0.40f, 0.25f, 0.25f));

	// Init resourceHandler
	resourceHandler->init();

	// Set mouse properties
	sf::Mouse::setPosition(sf::Vector2i((window.getView().getSize().x / 2), (window.getView().getSize().y / 2)), window); // Default mouse location
	window.setMouseCursorVisible(false);

	// Init Menu;
	menu = (std::unique_ptr<Menu>(new Menu(window, state, resourceHandler)));

	// Start Gameloop
	this->runGame();
}

/// <summary>
/// Runs the game loop
/// </summary>
void GameEngine::runGame()
{
	// While the game is open
	while (window.isOpen())
	{
		this->input(); // Process input

		// Add elapsed time with the gameclock time between last frame.
		this->elapsedTime += this->gameClock.restart();

		// Game Loop
		while (this->elapsedTime >= timeStep)
		{
			// Process the game data
			this->process();

			// Subtract time of this frame.
			this->elapsedTime -= timeStep;
		}

		// Draw the game
		this->draw();

	}
}

/// <summary>
/// Draws the game (executed from game loop)
/// </summary>
void GameEngine::draw()
{
	window.clear(sf::Color::Black);

	// Game States
	if (getState() == GameState::GAME)
	{
		window.setView(playerBar);
		this->world->drawStats();
		this->drawOpts();

		window.setView(mainView);
		this->world->draw();

		// CHECK IF IT IS GAME OVER or GAME WIN
		if (world->isGameOver() == 1)
		{
			setState(GameState::GAMEOVER);
			menu->resetCurrentOption();
		}
		else if (world->isGameOver() == 2)
		{
			setState(GameState::GAMEWIN);
			menu->resetCurrentOption();
		}
	}

	// Pause And Game Over state
	else if (getState() == GameState::PAUSE ||
		getState() == GameState::GAMEOVER ||
		getState() == GameState::GAMEWIN)
	{
		window.setView(playerBar);
		this->world->drawStats();

		window.setView(mainView);
		this->world->draw();

		window.setView(fullView);
		sf::RectangleShape darkOverLay(sf::Vector2f(window.getView().getSize()));
		darkOverLay.setFillColor(sf::Color(0, 0, 0, 150));
		darkOverLay.setPosition(0, 0);
		window.draw(darkOverLay);

		this->menu->draw();

		// Draw the Game Over with the following offset
		if (getState() == GameState::GAMEWIN)
			this->menu->drawPause((window.getView().getSize().x / 3), 100 + (window.getView().getSize().y / 2) * -1); // Small workaround so we dont have to take in offset into ->draw();
		if (getState() == GameState::PAUSE || getState() == GameState::GAMEOVER)
			this->menu->drawPause((window.getView().getSize().x / 3), (window.getView().getSize().y / 2) * -1); // Small workaround so we dont have to take in offset into ->draw();

	}

	// Menu states
	else if (getState() == GameState::MAIN_MENU ||
		getState() == GameState::STAGE_SELECT ||
		getState() == GameState::DIFFICULTY_SELECT ||
		getState() == GameState::CREDITS ||
		getState() == GameState::HIGHSCORE)
	{
		window.setView(fullView);
		this->world->draw();
		this->menu->draw();
		this->drawOpts();
	}

	window.display();

}

/// <summary>
/// Processes the game engine (from game loop)
/// </summary>
void GameEngine::process()
{
	// Process the Game State
	if (getState() == GameState::GAME)
	{
		this->world->input(this->event);
		this->world->process();
		return;
	}

	// Process the game initialization (This should only run once
	else if (getState() == GameState::INIT_GAME)
	{
		LOGD("Initializing a new game.");
		setState(GameState::GAME);
		resourceHandler->stopAllSound();
		world.reset(new World(
			window,
			resourceHandler,
			timeStep,
			false,
			menu->getStageSelectOption(),
			menu->getHardmodeSelected(),

			// This checks if DEATH_STAR theme is selected, and selects the corresponding music. Consider revising, Bad sctructure wise
			(menu->getStageSelectOption() - 1 == ResourceHandler::Scripts::DEATH_STAR) ?
			resourceHandler->getSound(ResourceHandler::Sound::MUSIC_DEATH_STAR_THEME) :
			resourceHandler->getSound(ResourceHandler::Sound::MUSIC_INGAME)));
	}
	else if (getState() == GameState::INIT_NEXT_STAGE)
	{
		menu->setStageSelectOption(menu->getStageSelectOption() + 1);
		setState(GameState::INIT_GAME);
	}

	// Process the Main menu initialization
	else if (getState() == GameState::INIT_MAIN_MENU)
	{
		// set new MOTD in the menu header
		std::srand(std::time(0));
		menu->setMessageOfTheDayId(std::rand() % resourceHandler->getMOTDSize());


		setState(GameState::MAIN_MENU);

		// Checks if the world exists, stops sound if so
		if (world != nullptr) resourceHandler->stopAllSound();

		// Reset the world
		world.reset(new World(window,
			resourceHandler,
			timeStep,
			true,
			-1,
			false,
			resourceHandler->getSound(ResourceHandler::Sound::MUSIC_MENU_SONG)));
	}

	// Process the GAEMOVER STATE
	else if (getState() == GameState::GAMEOVER)
	{
		world->stopSound();
	}

	// Process the Menu states
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

/// <summary>
/// process Input loop
/// </summary>
void GameEngine::input()
{
	while (window.pollEvent(this->event))
	{

		// Input event for each of the STATES
		if (getState() == GameState::MAIN_MENU
			|| getState() == GameState::STAGE_SELECT
			|| getState() == GameState::DIFFICULTY_SELECT
			|| getState() == GameState::PAUSE
			|| getState() == GameState::HIGHSCORE
			|| getState() == GameState::GAMEOVER
			|| getState() == GameState::GAMEWIN
			|| getState() == GameState::CREDITS)
		{
			this->menu->input(this->event);
		}

		// If the window close event is fired
		if (this->event.type == sf::Event::Closed)
		{
			this->window.close();
			exit(EXIT_SUCCESS);
		}

		// General game keys
		if (this->event.type == sf::Event::KeyReleased)
		{

			// Escape 
			if (this->event.key.code == sf::Keyboard::Escape){

				// If in GAME state
				if (getState() == GameState::GAME)
				{
					setState(GameState::PAUSE);
				}

				// If in pause state
				else if (getState() == GameState::PAUSE)
				{
					setState(GameState::GAME);
				}

				// Uodate current option according to the state
				menu->resetCurrentOption();
			}

			// IF m is clicked
			else if (this->event.key.code == sf::Keyboard::M) // Sound mute button
			{
				mute = !mute;
				resourceHandler->muteSound(mute);
			}


			else if (this->event.key.code == sf::Keyboard::N) // Fullscreen Toggle
			{
				window.clear();
				if (!fullscreen)
				{
					fullscreen = !fullscreen;
					window.create(sf::VideoMode(800, 600), "X1337", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize | sf::Style::Fullscreen);
				}
				else
				{
					fullscreen = !fullscreen;
					window.create(sf::VideoMode(800, 600), "X1337", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
				}
				window.setMouseCursorVisible(false);
			}

		}
	}
}

// State getter/setter
/// <summary>
/// Sets the state.
/// </summary>
/// <param name="state">The desired state.</param>
void GameEngine::setState(GameState state)
{
	this->state = state;
}

/// <summary>
/// Gets the state.
/// </summary>
/// <returns>Current state</returns>
GameState& GameEngine::getState()
{
	return this->state;
}


/// <summary>
/// Gets the resource handler.
/// </summary>
/// <returns>Resource Handler reference</returns>
std::shared_ptr<ResourceHandler>& GameEngine::getResourceHandler()
{
	return this->resourceHandler;
}

/// <summary>
/// Draws the mute button
/// </summary>
void GameEngine::drawOpts()
{

	//##################################//
	//######### Fullscreen #############//
	//##################################//
	sf::Sprite sprScreen;
	sprScreen.setTexture(resourceHandler->getTexture(ResourceHandler::Texture::MONITOR_ICON));
	sprScreen.setScale(0.30f, 0.30f);
	sprScreen.setPosition(window.getView().getSize().x - 150, window.getView().getSize().y - 24);
	window.draw(sprScreen);

	sf::Text txtScreen;
	txtScreen.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	txtScreen.setString("n");
	txtScreen.setColor(sf::Color(139, 137, 137));
	txtScreen.setCharacterSize(15);
	txtScreen.setPosition(sprScreen.getPosition().x + (sprScreen.getGlobalBounds().width / 2) - 4, sprScreen.getPosition().y - 3);
	window.draw(txtScreen);



	//##################################//
	//############ Mute ################//
	//##################################//

	sf::Text txtMute;
	txtMute.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	txtMute.setString("m");
	txtMute.setColor(sf::Color(139, 137, 137));
	txtMute.setCharacterSize(15);
	txtMute.setPosition(sprScreen.getPosition().x + sprScreen.getGlobalBounds().width + 10, sprScreen.getPosition().y);
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
	sprMute.setPosition(txtMute.getPosition().x + 15, txtMute.getPosition().y);
	window.draw(sprMute);



}
