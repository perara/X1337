#pragma once
#include <SFML\Graphics\RenderWindow.hpp>

#include "Player.h"
#include "World.h"
#include "GameGlobals.h"
#include "Button.h"

class GameEngine{
public:
	GameEngine();
	void runGame();
	void gameLoop();
	sf::RenderWindow window;
	World world;
};