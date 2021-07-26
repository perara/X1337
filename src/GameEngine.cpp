#include "../include/GameEngine.h"
#include "../include/Log.h"
#include "effolkronium/random.hpp"
#include <memory>
#include <spdlog/spdlog.h>

/// <summary>
/// Initializes a new instance of the <see cref="GameEngine"/> class.
/// </summary>
GameEngine::GameEngine() :
// Declare the Window
renderer(800, 600, "X1337"),
resourceHandler(std::make_shared<ResourceManager>(renderer)), // Create a new Resource Handler (smart_ptr)
timeStep(sf::seconds(1.0f / 240.f)), // Set timestep to 60 FPS
elapsedTime(timeStep),
event(sf::Event()),
mute(false),
mainView(sf::View(sf::FloatRect(0, 0, 800, 600))),
fullView(sf::View(sf::FloatRect(0, 0, static_cast<float>(renderer.getSize().x), static_cast<float>(renderer.getSize().y)))),
menuGameDemoView(sf::View(sf::FloatRect(0, 0, 800, 600))),
playerBar(sf::View(sf::FloatRect(0, 0, 800, 30))),

state(GameState::INIT_MAIN_MENU),
fullscreen(false)
{
	// Window configuration
	//window.setFramerateLimit(120);

	//####################################//
	//#########View Declaration###########//
	//####################################//
	fullView.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	renderer.setView(fullView);

	mainView.setViewport(sf::FloatRect(0, 0, 1, 0.95f));
	playerBar.setViewport(sf::FloatRect(0, 0.95f, 1.0f, 0.05f));
	menuGameDemoView.setViewport(sf::FloatRect(0.40f, 0.40f, 0.25f, 0.25f));

	// Init resourceHandler
	resourceHandler->init();

	// Set mouse properties
	/*sf::Mouse::setPosition(sf::Vector2i(
	        (int)(renderer.getView().getSize().x / 2.0),
	        (int)(renderer.getView().getSize().y / 2.0)),
                           renderer.getWindow()); // Default mouse location*/
	renderer.getWindow().setMouseCursorVisible(false);

	// Init Menu;
	menu = std::make_unique<Menu>(renderer, state, resourceHandler);
}

void GameEngine::gameStep(){
    this->input(); // Process input

    // Add elapsed time with the gameclock time between last frame.
    this->elapsedTime += this->gameClock.restart();


    // Game Loop
    if (this->elapsedTime < timeStep) {
        return;
    }


    // Process the game data
    this->process();

    // Subtract time of this frame.
    this->elapsedTime -= timeStep;


    // Draw the game
    this->draw();
}

/// <summary>
/// Runs the game loop
/// </summary>
void GameEngine::runGame()
{
    this->gameClock.restart();

	// While the game is open
	while (renderer.isOpen())
	{
        gameStep();
	}
}

/// <summary>
/// Draws the game (executed from game loop)
/// </summary>
void GameEngine::draw()
{

	renderer.clear(sf::Color::Black);

	// Game States
	if (getState() == GameState::GAME)
	{
		renderer.setView(playerBar);
		this->world->drawStats();
		this->drawOpts();

		renderer.setView(mainView);
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
		renderer.setView(playerBar);
		this->world->drawStats();

		renderer.setView(mainView);
		this->world->draw();

		renderer.setView(fullView);
		sf::RectangleShape darkOverLay(sf::Vector2f(renderer.getView().getSize()));
		darkOverLay.setFillColor(sf::Color(0, 0, 0, 150));
		darkOverLay.setPosition(0, 0);
		renderer.draw(darkOverLay);

		this->menu->draw();

		// Draw the Game Over with the following offset
		if (getState() == GameState::GAMEWIN)
			this->menu->drawPause(
                    renderer.getView().getSize().x / 3,
                    100 + (renderer.getView().getSize().y / 2) * -1); // Small workaround so we dont have to take in offset into ->draw();
		if (getState() == GameState::PAUSE || getState() == GameState::GAMEOVER)

			this->menu->drawPause(
			        (renderer.getView().getSize().x / 3),
                    ((renderer.getView().getSize().y / 2) * -1)); // Small workaround so we dont have to take in offset into ->draw();

	}

	// Menu states
	else if (getState() == GameState::MAIN_MENU ||
		getState() == GameState::STAGE_SELECT ||
		getState() == GameState::DIFFICULTY_SELECT ||
		getState() == GameState::CREDITS ||
		getState() == GameState::HIGHSCORE)
	{
		renderer.setView(fullView);
		this->world->draw();
		this->menu->draw();
		this->drawOpts();
	}

	renderer.display();

}

/// <summary>
/// Processes the game engine (from game loop)
/// </summary>
void GameEngine::process()
{
	// Process the Game State
	if (getState() == GameState::GAME)
	{
		this->world->input(event);
		this->world->process();
		return;
	}

	// Process the game initialization (This should only run once
	else if (getState() == GameState::INIT_GAME)
	{
        SPDLOG_INFO("Initializing a new game.");
		setState(GameState::GAME);
		resourceHandler->stopAllSound();
		world = std::make_unique<World>(
                renderer,
                resourceHandler,
                timeStep,
                (Constants::ResourceC::Scripts)menu->getStageSelectOption(),
                menu->getHardmodeSelected(),
			// This checks if DEATH_STAR theme is selected, and selects the corresponding music. Consider revising, Bad sctructure wise
			(menu->getStageSelectOption() - 1 == Constants::ResourceC::Scripts::DEATH_STAR) ?
			resourceHandler->getSound(Constants::ResourceC::Sound::MUSIC_DEATH_STAR_THEME) :
			resourceHandler->getSound(Constants::ResourceC::Sound::MUSIC_INGAME));
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
		menu->setMessageOfTheDayId(
		        effolkronium::random_static::get(0, resourceHandler->getMOTDSize())
		        );


		setState(GameState::MAIN_MENU);

		// Checks if the world exists, stops sound if so
		if (world != nullptr) resourceHandler->stopAllSound();

		// Reset the world
		world = std::make_unique<World>(renderer,
                                        resourceHandler,
                                        timeStep,
                                        Constants::ResourceC::Scripts::GAME_MENU,
                                        false,
                                        resourceHandler->getSound(Constants::ResourceC::Sound::MUSIC_MENU_SONG));
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

void GameEngine::CreateInputEvent(Constants::Input inputEnum) {
    sf::Event e{};

#define CaseKey(keyPress, presstype, keyboardVal)\
case(Constants::Input::keyPress):{\
    e.type = sf::Event::presstype; \
    e.key = sf::Event::KeyEvent{sf::Keyboard::keyboardVal, false, false, false, false};\
    break;}\


    switch (inputEnum) {
        CaseKey(Key1Press, KeyPressed, Numpad0)
        CaseKey(Key1Release,KeyReleased , Numpad0)
        CaseKey(Key2Press,KeyPressed , Numpad1)
        CaseKey(Key2Release, KeyReleased, Numpad1)
        CaseKey(KeyLeftPress, KeyPressed, Left)
        CaseKey(KeyLeftRelease, KeyReleased, Left)
        CaseKey(KeyRightPress, KeyPressed, Right)
        CaseKey(KeyRightRelease,KeyReleased , Right)
        CaseKey(KeyUpPress,KeyPressed ,Up )
        CaseKey(KeyUpRelease,KeyReleased ,Up )
        CaseKey(KeyDownPress, KeyPressed, Down)
        CaseKey(KeyDownRelease, KeyReleased, Down)
        CaseKey(KeyMPress, KeyPressed, M)
        CaseKey(KeyMRelease, KeyReleased, M)
        CaseKey(KeyEscapePress, KeyPressed, Escape)
        CaseKey(KeyEscapeRelease,KeyReleased , Escape)
        CaseKey(KeyEnterPress, KeyPressed, Enter)
        CaseKey(KeyEnterRelease, KeyReleased, Enter)
        case(Constants::Input::VoidKey):
            break;
        case(Constants::Input::NumInputs):
            break;
    }

    renderer.injectedEvents.push_back(std::make_shared<sf::Event>(e));
}


/// <summary>
/// process Input loop
/// </summary>
void GameEngine::input()
{
	while (renderer.pollEvent(this->event))
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
			this->renderer.close();
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
				renderer.clear();
				if (!fullscreen)
				{
					fullscreen = !fullscreen;
					renderer.create(sf::VideoMode(800, 600), "X1337", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize | sf::Style::Fullscreen);
				}
				else
				{
					fullscreen = !fullscreen;
					renderer.create(sf::VideoMode(800, 600), "X1337", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
				}
				renderer.getWindow().setMouseCursorVisible(false);
			}

		}
	}
}

// State getter/setter
/// <summary>
/// Sets the state.
/// </summary>
/// <param name="state">The desired state.</param>
void GameEngine::setState(GameState _state)
{
	this->state = _state;
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
/// Draws the mute button
/// </summary>
void GameEngine::drawOpts()
{

	//##################################//
	//######### Fullscreen #############//
	//##################################//
	sf::Sprite sprScreen;
	sprScreen.setTexture(resourceHandler->getTexture(Constants::ResourceC::Texture::MONITOR_ICON));
	sprScreen.setScale(0.30f, 0.30f);
	sprScreen.setPosition(renderer.getView().getSize().x - 150, renderer.getView().getSize().y - 24);
	renderer.draw(sprScreen);

	sf::Text txtScreen;
	txtScreen.setFont(resourceHandler->getFont(Constants::ResourceC::Fonts::SANSATION));
	txtScreen.setString("n");
	txtScreen.setFillColor(sf::Color(139, 137, 137));
	txtScreen.setCharacterSize(15);
	txtScreen.setPosition(sprScreen.getPosition().x + (sprScreen.getGlobalBounds().width / 2) - 4, sprScreen.getPosition().y - 3);
	renderer.draw(txtScreen);



	//##################################//
	//############ Mute ################//
	//##################################//

	sf::Text txtMute;
	txtMute.setFont(resourceHandler->getFont(Constants::ResourceC::Fonts::SANSATION));
	txtMute.setString("m");
	txtMute.setFillColor(sf::Color(139, 137, 137));
	txtMute.setCharacterSize(15);
	txtMute.setPosition(sprScreen.getPosition().x + sprScreen.getGlobalBounds().width + 10, sprScreen.getPosition().y);
	renderer.draw(txtMute);

	sf::Sprite sprMute;
	if (mute)
	{
		sprMute.setTexture(resourceHandler->getTexture(Constants::ResourceC::Texture::AUDIO_OFF));
	}
	else
	{
		sprMute.setTexture(resourceHandler->getTexture(Constants::ResourceC::Texture::AUDIO_ON));
	}
	sprMute.setScale(0.20f, 0.20f);
	sprMute.setPosition(txtMute.getPosition().x + 15, txtMute.getPosition().y);
	renderer.draw(sprMute);



}
