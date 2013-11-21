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
	Object(sf::RenderWindow & window) : window(window){};
	virtual void draw();
	virtual bool process();
	GameShape* sprite;

protected:
	sf::RenderWindow& window;
};