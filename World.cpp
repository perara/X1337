#include "World.h"
#include <iostream>
World::World(sf::RenderWindow& window): Scene(window){
	// Add player objects
	Player *p1 = new Player(window, sf::Vector2f(100,250), 10);
	Player *p2 = new Player(window, sf::Vector2f(100,300), 10);
	
	this->addObject(p1);
	this->addObject(p2);
}
