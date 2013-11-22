#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\System\Time.hpp>


class World;



/// <summary>
/// This is the game engine class which processes everything in the game. It holds the statemachine which decides current state.
/// </summary>
class GameEngine{
public:
	GameEngine();
	void runGame();
	void gameLoop();
	sf::RenderWindow window;
	World* world;
	
	sf::Time elapsedTime;
	sf::Clock gameClock;
};