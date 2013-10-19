#pragma once
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

class Object{
public:
	Object();
	Object(sf::RenderWindow & window) : window(window){};
	virtual void draw();
	virtual void process();
	virtual void inputHandler() = 0;

protected:
	sf::CircleShape* sprite;
	sf::RenderWindow & window;
};