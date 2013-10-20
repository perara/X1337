#pragma once
#include "Object.h"
#include <SFML\Window\Event.hpp>
#include "BulletFactory.h"

class Player : public Object{
public:
	BulletFactory* bFactory;
	Player(){}
	Player(sf::RenderWindow& window, sf::Vector2f pos, int radius);
	void inputHandler();
	void process();


};