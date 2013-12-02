#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\System\Time.hpp>
#include <SFML\Graphics.hpp>
#include <memory>
#include "World.h"

class Menu;

#define GameState GameEngine::State

/// <summary>
/// This is the game engine class which processes everything in the game. It holds the statemachine which decides current state.
/// </summary>
class GameEngine{
public:
	GameEngine();

	enum State{
		GAME, 
		INIT_GAME,
		INIT_MAIN_MENU,
		STAGE_SELECT,
		DIFFICULTY_SELECT,
		MAIN_MENU, 
		OPTIONS, 

		HIGHSCORE,

		PAUSE,
		GAMEOVER

	};

	// State getter/setter
	void setState(GameState state);
	GameState& getState();

	// Resource Handler getter/setter
	std::unique_ptr<ResourceHandler>& GameEngine::getResourceHandler();

private:
	void runGame();
	void gameLoop();
	void pollInput();
	sf::RenderWindow window;

	std::unique_ptr<World> world;
	std::unique_ptr<Menu> menu;
	std::unique_ptr<ResourceHandler> resourceHandler;

	const sf::Time timeStep;
	sf::Time elapsedTime;
	sf::Clock gameClock;
	sf::Event event;


	// VIEWS
	sf::View mainView;
	sf::View fullScreen;
	sf::View menuGameDemoView;
	sf::View playerStatsView;

	State state;
};