#include "World.h"


World::World():
	windowSize(800, 800),
	p1(sf::Vector2f(windowSize.x/2, windowSize.y-100), 5)
{
}


World::~World(void)
{
}
