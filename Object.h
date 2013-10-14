#pragma once
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

class Object{
public:

	virtual void draw(sf::RenderWindow &window);
	virtual void process();
	virtual void inputHandler(sf::RenderWindow &window) = 0;

protected:
	sf::CircleShape* sprite;
};