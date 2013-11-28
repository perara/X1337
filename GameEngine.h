#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\System\Time.hpp>
#include <SFML\Graphics.hpp>
#include <memory>

class World;
class Menu;



/// <summary>
/// This is the game engine class which processes everything in the game. It holds the statemachine which decides current state.
/// </summary>
class GameEngine{
public:
	GameEngine();

private:
	void runGame();
	void gameLoop();
	void pollInput();
	sf::RenderWindow window;

	std::unique_ptr<World> world;
	std::unique_ptr<Menu> menu;

	sf::Time elapsedTime;
	sf::Clock gameClock;
	sf::Event event;


	// VIEWS
	sf::View mainView;
	sf::View fullScreen;
	sf::View menuGameDemoView;
	sf::View playerStatsView;
};