#pragma once
#include "Log.h"
#include "GameShape.h"

#include <SFML/Window.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

/// <summary>
/// Abstract base class for all objects in the game. A object can for example be a Player, Bullet or a Enemy.
/// </summary>
class Object
{
public:
	Object();
	Object(sf::RenderWindow & window) : window(window){deleted = false;};
	virtual void draw();
	virtual void process();
	GameShape* sprite;
	bool getDeleted();

protected:
	sf::RenderWindow& window;
	bool deleted;
};