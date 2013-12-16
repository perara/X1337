#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\System\Time.hpp>
#include <SFML\Graphics.hpp>
#include <memory>
#include "World.h"

class Menu;

#define GameState GameEngine::State

/// <summary>
/// GameEngine runs the game. This class serves a game loop which runs until game end. All state is handler in this class.
/// </summary>
class GameEngine{
public:
	GameEngine();

	/// State Enumerable which contains all possible states for the GameEngine, World and Menu.
	enum State{
		GAME,
		INIT_GAME,
		INIT_MAIN_MENU,
		STAGE_SELECT,
		DIFFICULTY_SELECT,
		MAIN_MENU,
		OPTIONS,
		CREDITS,
		HIGHSCORE,
		PAUSE,
		GAMEOVER,


		GAMEWIN,
		INIT_NEXT_STAGE,

	};

	// State getter/setter
	void setState(GameState state);
	GameState& getState();

	// Resource Handler getter/setter
	std::shared_ptr<ResourceHandler>& GameEngine::getResourceHandler();

private:
	void runGame();
	void gameLoop();

	void process();
	void draw();
	void input();

	void drawOpts(); // Function which draws the mute icon

	sf::RenderWindow window;

	std::unique_ptr<World> world;
	std::unique_ptr<Menu> menu;
	std::shared_ptr<ResourceHandler> resourceHandler;

	const sf::Time timeStep;
	sf::Time elapsedTime;
	sf::Clock gameClock;
	sf::Event event;
	bool mute; // State of the sound

	// VIEWS
	sf::View mainView;
	sf::View fullView;
	sf::View menuGameDemoView;
	sf::View playerBar;

	State state;
	bool fullscreen;
};