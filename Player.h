#pragma once
#include "Object.h"
#include <SFML\Window\Event.hpp>

class Player : public Object
{
public:
	Player(){}
	Player(sf::Vector2f pos, int radius);
	void inputHandler(sf::RenderWindow &window) ;


};