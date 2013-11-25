#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\System\Time.hpp>
#include <SFML\Graphics.hpp>


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

	World* world;
	Menu* menu;

	sf::Time elapsedTime;
	sf::Clock gameClock;
	sf::Event event;
};