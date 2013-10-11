#include <SFML\Graphics.hpp>
#include "Player.h"
#pragma once
class World
{
public:
	World(void);
	~World(void);
	sf::Vector2f windowSize;
	Player p1;
};

