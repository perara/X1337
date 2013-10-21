#pragma once
#include "Log.h"

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <vector>

class Object{
public:
	Object();
	Object(sf::RenderWindow & window) : window(window){};
	virtual void draw();
	virtual bool process();


protected:
	sf::CircleShape* sprite;
	sf::RenderWindow& window;
};