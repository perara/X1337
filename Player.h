#pragma once
#include "Object.h"
#include <SFML\Window\Event.hpp>

class Player : public Object{
public:
	Player(){}
	Player(sf::RenderWindow & window, sf::Vector2f pos, int radius);
	void inputHandler();
	void process();
};